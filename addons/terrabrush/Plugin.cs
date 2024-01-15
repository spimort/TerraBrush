#if TOOLS
using Godot;
using Microsoft.VisualBasic;
using System;
using System.Collections.Generic;
using System.Linq;

namespace TerraBrush;

[Tool]
public partial class Plugin : EditorPlugin {
    private const float UpdateDelay = 0.005f;

	private TerrainControlDock _terrainControlDock;
    private PackedScene _terrainControlDockPrefab;
    private PackedScene _toolsPieMenuPrefab;
    private PackedScene _customContentPieMenuPrefab;
    private BrushDecal _brushDecal;
    private TerraBrush _currentTerraBrushNode;
    private bool _isMousePressed;
    private Vector3 _mouseHitPosition;
    private float _updateTime = 0;
    private EditorUndoRedoManager _undoRedo;
    private bool _preventInitialDo = false;
    private Node _editorViewportsContainer = null;
    private Control[] _editorViewports = null;
    private Control _overlaySelector = null;
    private Button _updateTerrainSettingsButton = null;

    private void CreateCustomSetting(string name, Variant defaultValue, Variant.Type type, PropertyHint hint = PropertyHint.None, string hintString = null) {
        if (ProjectSettings.HasSetting(name)) {
            return;
        }

        var propertyInfo = new Godot.Collections.Dictionary() {
            {"name", name},
            {"type", (int)type},
            {"hint", (int)hint},
            {"hint_string", hintString}
        };

        ProjectSettings.SetSetting(name, defaultValue);
        ProjectSettings.AddPropertyInfo(propertyInfo);
        ProjectSettings.SetInitialValue(name, defaultValue);
    }

    public override void _EnterTree() {
        var keybindManager = new KeybindManager();
		var script = GD.Load<Script>("res://addons/terrabrush/TerraBrush.cs");
		var icon = GD.Load<Texture2D>("res://addons/terrabrush/icon.png");

		AddCustomType("TerraBrush", "Node3D", script, icon);
        CreateCustomSetting(SettingContants.DecalColor, Colors.Red, Variant.Type.Color);
        CreateCustomSetting(SettingContants.CustomBrushesFolder, "res://TerraBrush_CustomBrushes", Variant.Type.String);
        AddInspectorPlugin(new ButtonInspectorPlugin());

		_terrainControlDockPrefab = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/TerrainControlDock.tscn");
        _toolsPieMenuPrefab = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/ToolsPieMenu.tscn");
        _customContentPieMenuPrefab = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/CustomContentPieMenu.tscn");
        _editorViewportsContainer = GetEditorViewportsContainer();
        _editorViewports = _editorViewportsContainer.GetChildren().Select(viewport => (Control) viewport).ToArray();

        keybindManager.RegisterInputMap();
        keybindManager.LoadEditorSettings();
        AddToolMenuItem("TerraBrush Key bindings", Callable.From(HandleKeyBindings));
    }

    private void HandleKeyBindings() {
        var dlg = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/KeybindSettings.tscn")
            .Instantiate<KeybindSettings>();
        dlg.Confirmed += () => dlg.QueueFree();
        GetTree().Root.AddChild(dlg);
        dlg.PopupCentered();
    }

    public override void _Edit(GodotObject @object) {
        base._Edit(@object);

        if (_Handles(@object)) {
            OnEditTerrainNode((TerraBrush) @object);
        } else {
            OnExitEditTerrainNode();
        }
    }

    public override bool _Handles(GodotObject @object) {
        return @object is TerraBrush;
    }

    public override void _SaveExternalData() {
        base._SaveExternalData();

        _currentTerraBrushNode?.SaveResources();
    }

    public override int _Forward3DGuiInput(Camera3D viewportCamera, InputEvent @event) {
        var preventGuiInput = false;

        if (@event is InputEventMouseMotion inputMotion) {
            var meshPosition = GetRayCastWithTerrain(viewportCamera, inputMotion.Position);

            if (meshPosition == Vector3.Inf) {
                _brushDecal.Visible = false;
            } else {
                _brushDecal.Visible = true;
                _brushDecal.Position = new Vector3(meshPosition.X, 0, meshPosition.Z);
            }

            _mouseHitPosition = meshPosition;
        }

        if (@event is InputEventKey inputEvent) {
            _terrainControlDock.SetShiftPressed(Input.IsKeyPressed(Key.Shift));

            if (!inputEvent.Pressed || inputEvent.Echo) return base._Forward3DGuiInput(viewportCamera, @event);

            if (inputEvent.IsAction(KeybindManager.StringNames.ToolPie)) {
                ShowToolPieMenu();
                return (int) EditorPlugin.AfterGuiInput.Stop;
            }

            if (inputEvent.IsAction(KeybindManager.StringNames.BrushPie)) {
                ShowCustomContentPieMenu("Brushes", customContentPieMenu => {
                    CustomContentLoader.AddBrushesPreviewToParent(customContentPieMenu.PieMenu, index => {
                        _terrainControlDock.SetSelectedBrushIndex(index);
                        HideOverlaySelector();
                    });
                });
                return (int) EditorPlugin.AfterGuiInput.Stop;
            }

            if (inputEvent.IsAction(KeybindManager.StringNames.ToolContentPie)) {
                ShowCurrentToolCustomContentPieMenu();
                return (int) EditorPlugin.AfterGuiInput.Stop;
            }

            if (inputEvent.IsAction(KeybindManager.StringNames.BrushSizeSelector)) {
                ShowBrushNumericSelector(1, 200, Colors.LimeGreen, _currentTerraBrushNode.BrushSize, value => {
                    _terrainControlDock.SetBrushSize(value);
                });

                return (int) EditorPlugin.AfterGuiInput.Stop;
            }

            if (inputEvent.IsAction(KeybindManager.StringNames.BrushStrengthSelector)) {
                ShowBrushNumericSelector(1, 200, Colors.Crimson, (int) (_currentTerraBrushNode.BrushStrength * 100), value => {
                    _terrainControlDock.SetBrushStrength(value / 100.0f);
                });

                return (int) EditorPlugin.AfterGuiInput.Stop;
            }

            if (inputEvent.IsAction(KeybindManager.StringNames.EscapeSelector) && _overlaySelector != null) {
                HideOverlaySelector();
                return (int) EditorPlugin.AfterGuiInput.Stop;
            }
        }

        if (@event is InputEventMouseButton inputButton) {
            if (_overlaySelector != null) {
                if (_overlaySelector is BrushNumericSelector brushNumericSelector) {
                    brushNumericSelector.RequestSelectValue();
                }

                HideOverlaySelector();
                return (int) EditorPlugin.AfterGuiInput.Stop;
            }

            if (inputButton.ButtonIndex == MouseButton.Left) {
                if (inputButton.Pressed) {
                    if (_mouseHitPosition != Vector3.Inf) {
                        _undoRedo.CreateAction("Modify terrain");

                        // Trigger a dirty state
                        _undoRedo.AddUndoProperty(_currentTerraBrushNode, nameof(TerraBrush.TerrainSize), _currentTerraBrushNode.TerrainSize);

                        _undoRedo.AddUndoMethod(this, nameof(OnUndoTexture), _currentTerraBrushNode.HeightMap, _currentTerraBrushNode.HeightMap.GetImage().GetData());

                        if (_currentTerraBrushNode.WaterTexture != null) {
                            _undoRedo.AddUndoMethod(this, nameof(OnUndoTexture), _currentTerraBrushNode.WaterTexture, _currentTerraBrushNode.WaterTexture.GetImage().GetData());
                        }

                        if (_currentTerraBrushNode.SnowTexture != null) {
                            _undoRedo.AddUndoMethod(this, nameof(OnUndoTexture), _currentTerraBrushNode.SnowTexture, _currentTerraBrushNode.SnowTexture.GetImage().GetData());
                        }

                        if (_currentTerraBrushNode.Splatmaps != null) {
                            foreach (var splatmap in _currentTerraBrushNode.Splatmaps) {
                                _undoRedo.AddUndoMethod(this, nameof(OnUndoTexture), splatmap, splatmap.GetImage().GetData());
                            }
                        }

                        if (_currentTerraBrushNode.Foliages != null) {
                            foreach (var foliage in _currentTerraBrushNode.Foliages) {
                                _undoRedo.AddUndoMethod(this, nameof(OnUndoTexture), foliage.Texture, foliage.Texture.GetImage().GetData());
                            }
                        }

                        if (_currentTerraBrushNode.Objects != null) {
                            foreach (var objectItem in _currentTerraBrushNode.Objects) {
                                _undoRedo.AddUndoMethod(this, nameof(OnUndoTexture), objectItem.Texture, objectItem.Texture.GetImage().GetData());
                            }
                        }

                        _isMousePressed = true;
                        preventGuiInput = true;
                    }
                } else if (_isMousePressed) {
                    _currentTerraBrushNode.Terrain.TerrainUpdated(true);
                    _isMousePressed = false;

                    // Trigger a dirty state
                    _undoRedo.AddDoProperty(_currentTerraBrushNode, nameof(TerraBrush.TerrainSize), _currentTerraBrushNode.TerrainSize);

                    _undoRedo.AddDoMethod(this, nameof(OnUndoTexture), _currentTerraBrushNode.HeightMap, _currentTerraBrushNode.HeightMap.GetImage().GetData());

                    if (_currentTerraBrushNode.WaterTexture != null) {
                        _undoRedo.AddDoMethod(this, nameof(OnUndoTexture), _currentTerraBrushNode.WaterTexture, _currentTerraBrushNode.WaterTexture.GetImage().GetData());
                    }

                    if (_currentTerraBrushNode.SnowTexture != null) {
                        _undoRedo.AddDoMethod(this, nameof(OnUndoTexture), _currentTerraBrushNode.SnowTexture, _currentTerraBrushNode.SnowTexture.GetImage().GetData());
                    }

                    if (_currentTerraBrushNode.Splatmaps != null) {
                        foreach (var splatmap in _currentTerraBrushNode.Splatmaps) {
                            _undoRedo.AddDoMethod(this, nameof(OnUndoTexture), splatmap, splatmap.GetImage().GetData());
                        }
                    }

                    if (_currentTerraBrushNode.Foliages != null) {
                        foreach (var foliage in _currentTerraBrushNode.Foliages) {
                            _undoRedo.AddDoMethod(this, nameof(OnUndoTexture), foliage.Texture, foliage.Texture.GetImage().GetData());
                        }
                    }

                    if (_currentTerraBrushNode.Objects != null) {
                        foreach (var objectItem in _currentTerraBrushNode.Objects) {
                            _undoRedo.AddDoMethod(this, nameof(OnUndoTexture), objectItem.Texture, objectItem.Texture.GetImage().GetData());
                        }
                    }

                    _undoRedo.AddUndoMethod(this, nameof(OnUndoRedo));
                    _undoRedo.AddDoMethod(this, nameof(OnUndoRedo));

                    _preventInitialDo = true;
                    _undoRedo.CommitAction();
                    _preventInitialDo = false;
                }
            }
        }

        _currentTerraBrushNode?.UpdateCameraPosition(viewportCamera);

        if (preventGuiInput) {
            return (int) EditorPlugin.AfterGuiInput.Stop;
        } else {
            return base._Forward3DGuiInput(viewportCamera, @event);
        }
    }

    public override void _PhysicsProcess(double delta) {
        base._PhysicsProcess(delta);

        if (!_isMousePressed) {
            _updateTime = 0;
        } else if (_updateTime > 0) {
            _updateTime -= (float) delta;
        } else if (_isMousePressed && _mouseHitPosition != Vector3.Inf) {
            _currentTerraBrushNode.EditTerrain(_mouseHitPosition);

            _updateTime = UpdateDelay;
        }
    }

    private void OnUndoTexture(ImageTexture imageTexture, byte[] previousImageData) {
        if (_preventInitialDo) {
            return;
        }

        var image = new Image();
        image.SetData(imageTexture.GetWidth(), imageTexture.GetHeight(), imageTexture.GetImage().HasMipmaps(), imageTexture.GetFormat(), previousImageData);
        imageTexture.Update(image);
    }

    private async void OnUndoRedo() {
        if (_preventInitialDo) {
            return;
        }

        _currentTerraBrushNode.Terrain.TerrainUpdated(true);
        _currentTerraBrushNode.Terrain.TerrainSplatmapsUpdated();
        _currentTerraBrushNode.UpdateFoliagesGroudTexture();

        _currentTerraBrushNode.ClearObjects();
        await _currentTerraBrushNode.CreateObjects();
    }

    private Vector3 GetRayCastWithTerrain(Camera3D editorCamera, Vector2 mousePosition) {
        if (editorCamera.GetViewport() is SubViewport viewport && viewport.GetParent() is SubViewportContainer viewportContainer) {
            var screenPosition = mousePosition * viewport.Size / viewportContainer.Size;

            var from = editorCamera.ProjectRayOrigin(screenPosition);
            var dir = editorCamera.ProjectRayNormal(screenPosition);

            var distance = editorCamera.Far * 1.2f;
            var spaceState = _currentTerraBrushNode.GetWorld3D().DirectSpaceState;

            var query = new PhysicsRayQueryParameters3D();
            query.From = from;
            query.To = from + dir * distance;
            var result = spaceState.IntersectRay(query);

            if (result?.Count > 0 && result["collider"].Obj == _currentTerraBrushNode.Terrain?.TerrainCollider) {
                return (Vector3)result["position"] + new Vector3(0, 0.1f, 0);
            }
        }

        return Vector3.Inf;
    }

	public override void _ExitTree() {
		RemoveCustomType("TerraBrush");
        RemoveToolMenuItem("TerraBrush Key bindings");

        OnExitEditTerrainNode();
	}

    private void RemoveDock() {
		if (_terrainControlDock != null) {
			RemoveControlFromDocks(_terrainControlDock);
			_terrainControlDock.Free();

            _terrainControlDock = null;
		}

        if (_updateTerrainSettingsButton != null) {
            RemoveControlFromContainer(CustomControlContainer.SpatialEditorMenu, _updateTerrainSettingsButton);
            _updateTerrainSettingsButton.QueueFree();

            _updateTerrainSettingsButton = null;
        }
    }

    private void OnEditTerrainNode(TerraBrush terraBrush) {
        RemoveDock();
        GetNodeOrNull("BrushDecal")?.QueueFree();
        _brushDecal?.QueueFree();

        _brushDecal = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/BrushDecal.tscn").Instantiate<BrushDecal>();
        _brushDecal.Name = "BrushDecal";
        AddChild(_brushDecal);

        _brushDecal.SetSize(terraBrush.BrushSize);
        _brushDecal.SetBrushImage(terraBrush.BrushImage);

        _currentTerraBrushNode = terraBrush;
        _currentTerraBrushNode.TerrainSettingsUpdated += () => {
            RemoveDock();
            AddDock();
        };
        _undoRedo = GetUndoRedo();

        AddDock();

        terraBrush.SetMeta("_edit_lock_", true);
    }

    private void AddDock() {
        _terrainControlDock = _terrainControlDockPrefab.Instantiate<TerrainControlDock>();
        _terrainControlDock.TerraBrush = _currentTerraBrushNode;
        _terrainControlDock.BrushDecal = _brushDecal;
        _terrainControlDock.EditorResourcePreview = EditorInterface.Singleton.GetResourcePreviewer();
        AddControlToDock(DockSlot.RightBl, _terrainControlDock);

        _updateTerrainSettingsButton = new Button();
        _updateTerrainSettingsButton.Text = "Update terrain";
        _updateTerrainSettingsButton.Connect("pressed", new Callable(this, nameof(UpdateTerrainSettings)));
        AddControlToContainer(CustomControlContainer.SpatialEditorMenu, _updateTerrainSettingsButton);
    }

    private void OnExitEditTerrainNode() {
        RemoveDock();
        HideOverlaySelector();

        _brushDecal?.QueueFree();
        _brushDecal = null;

        GetNodeOrNull("BrushDecal")?.QueueFree();

        _currentTerraBrushNode.SetMeta("_edit_lock_", false);

        _currentTerraBrushNode = null;
    }

    private Node GetEditorViewportsContainer() {
        return GetEditorViewportsContainerRecursive(EditorInterface.Singleton.GetBaseControl());
    }

    private Node GetEditorViewportsContainerRecursive(Node node) {
        foreach (var child in node.GetChildren()) {
            if (child.GetClass() == "Node3DEditorViewportContainer") {
                return child;
            }

            var subChild = GetEditorViewportsContainerRecursive(child);
            if (subChild != null) {
                return subChild;
            }
        }

        return null;
    }

    private Node GetActiveViewport() {
        return _editorViewports.FirstOrDefault(viewport => {
            return viewport.GetRect().HasPoint(viewport.GetLocalMousePosition());
        });
    }

    private void HideOverlaySelector() {
        if (_overlaySelector != null) {
            _overlaySelector.QueueFree();
            _overlaySelector = null;
        }
    }

    private void ShowToolPieMenu() {
        HideOverlaySelector();

        var activeViewport = GetActiveViewport();
        if (activeViewport != null) {
            var pieMenu = _toolsPieMenuPrefab.Instantiate<ToolsPieMenu>();
            pieMenu.OnToolSelected = toolType => {
                _terrainControlDock.SelectToolType(toolType);

                HideOverlaySelector();
            };

            _overlaySelector = pieMenu;

            _overlaySelector.Position = ((Control) activeViewport).GetGlobalMousePosition();

            EditorInterface.Singleton.GetBaseControl().AddChild(_overlaySelector);
        }
    }

    private void ShowCustomContentPieMenu(string label, Action<CustomContentPieMenu> addItems) {
        HideOverlaySelector();

        var activeViewport = GetActiveViewport();
        if (activeViewport != null) {
            var customContentPieMenu = _customContentPieMenuPrefab.Instantiate<CustomContentPieMenu>();

            _overlaySelector = customContentPieMenu;
            _overlaySelector.Position = ((Control) activeViewport).GetGlobalMousePosition();

            EditorInterface.Singleton.GetBaseControl().AddChild(_overlaySelector);

            addItems(customContentPieMenu);

            customContentPieMenu.PieMenu.Label = label;
            customContentPieMenu.PieMenu.UpdateContent();
        }
    }

    private void ShowCurrentToolCustomContentPieMenu() {
        switch (_currentTerraBrushNode.TerrainTool) {
            case TerrainToolType.Paint:
                ShowCustomContentPieMenu("Textures", customContentPieMenu => {
                    CustomContentLoader.AddTexturesPreviewToParent(_currentTerraBrushNode, customContentPieMenu.PieMenu, index => {
                        _terrainControlDock.SetSelectedTextureIndex(index);
                        HideOverlaySelector();
                    });
                });

                break;
            case TerrainToolType.FoliageAdd:
            case TerrainToolType.FoliagRemove:
                ShowCustomContentPieMenu("Foliages", customContentPieMenu => {
                    CustomContentLoader.AddFoliagesPreviewToParent(_currentTerraBrushNode, customContentPieMenu.PieMenu, index => {
                        _terrainControlDock.SetSelectedFoliageIndex(index);
                        HideOverlaySelector();
                    });
                });

                break;
            case TerrainToolType.ObjectAdd:
            case TerrainToolType.ObjectRemove:
                ShowCustomContentPieMenu("Objects", customContentPieMenu => {
                    CustomContentLoader.AddObjectsPreviewToParent(_currentTerraBrushNode, customContentPieMenu.PieMenu, index => {
                        _terrainControlDock.SetSelectedObjectIndex(index);
                        HideOverlaySelector();
                    });
                });

                break;
        }
    }

    private void ShowBrushNumericSelector(int minVale, int maxValue, Color widgetColor, int initialValue, Action<int> onValueSelected) {
        HideOverlaySelector();

        var activeViewport = GetActiveViewport();
        if (activeViewport != null) {
            var selectorPrefab = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/BrushNumericSelector.tscn");
            var selector = selectorPrefab.Instantiate<BrushNumericSelector>();

            selector.MinValue = minVale;
            selector.MaxValue = maxValue;
            selector.WidgetColor = widgetColor;

            _overlaySelector = selector;
            _overlaySelector.Position = ((Control) activeViewport).GetGlobalMousePosition();

            EditorInterface.Singleton.GetBaseControl().AddChild(_overlaySelector);

            selector.SetInitialValue(initialValue);

            selector.OnValueSelected = value => {
                onValueSelected(value);
                HideOverlaySelector();
            };

            selector.OnCancel = () => {
                HideOverlaySelector();
            };
        }
    }

    private void UpdateTerrainSettings() {
        _currentTerraBrushNode?.OnUpdateTerrainSettings();
    }
}
#endif
