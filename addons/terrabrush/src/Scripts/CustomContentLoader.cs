using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Godot;

namespace TerraBrush;

public static class CustomContentLoader {
    public static void AddBrushesPreviewToParent(Node parentNode, Action<int> onSelect, bool useCircleIcon = false) {
        var brushPreviewPrefab = ResourceLoaderHelper.Load<PackedScene>("res://addons/terrabrush/Components/DockPreviewButton.tscn");

        var directories = new List<DirAccess>() { DirAccess.Open("res://addons/terrabrush/Assets/Brushes/") };

        var customBrushesDirectory = (string)ProjectSettings.Singleton.GetSetting(SettingContants.CustomBrushesFolder);
        if (!string.IsNullOrWhiteSpace(customBrushesDirectory) && DirAccess.DirExistsAbsolute(customBrushesDirectory)) {
            directories.Add(DirAccess.Open(customBrushesDirectory));
        }

        var index = 0;
        foreach (var directory in directories) {
            foreach (var file in directory.GetFiles()) {
                if (file.EndsWith(".png", StringComparison.InvariantCultureIgnoreCase)) {
                    var brushImage = ResourceLoaderHelper.Load<Texture2D>(Path.Combine(directory.GetCurrentDir(), file));
                    var dockPreviewButton = brushPreviewPrefab.Instantiate<DockPreviewButton>();
                    dockPreviewButton.IconType = useCircleIcon ? IconType.Circle : IconType.Square;
                    dockPreviewButton.Margin = 5;
                    parentNode.AddChild(dockPreviewButton);

                    dockPreviewButton.ButtonImage = brushImage; // We keep the original image in the ButtonImage so we can use it later.

                    var whiteBrushImage = new Image();
                    whiteBrushImage.CopyFrom(brushImage.GetImage());

                    for (var x = 0; x < whiteBrushImage.GetWidth(); x++) {
                        for (var y = 0; y < whiteBrushImage.GetHeight(); y++) {
                            var pixel = whiteBrushImage.GetPixel(x, y);
                            whiteBrushImage.SetPixel(x, y, new Color(1.0f - pixel.R, 1.0f - pixel.G, 1.0f - pixel.B, pixel.A));
                        }
                    }

                    dockPreviewButton.SetTextureImage(ImageTexture.CreateFromImage(whiteBrushImage));

                    var currentIndex = index;
                    dockPreviewButton.OnSelect = () => {
                        onSelect(currentIndex);
                    };

                    index += 1;
                }
            }
        }
    }

    public static void AddTexturesPreviewToParent(TerraBrush terraBrush, Node parentNode, Action<int> onSelect, bool useCircleIcon = false) {
        if (terraBrush.TextureSets?.TextureSets != null) {
            var texturePreviewPrefab = ResourceLoaderHelper.Load<PackedScene>("res://addons/terrabrush/Components/DockPreviewButton.tscn");

            for (var i = 0; i < terraBrush.TextureSets.TextureSets.Length; i++) {
                var textureSet = terraBrush.TextureSets.TextureSets[i];

                var dockPreviewButton = texturePreviewPrefab.Instantiate<DockPreviewButton>();
                dockPreviewButton.IconType = useCircleIcon ? IconType.Circle : IconType.Square;
                dockPreviewButton.Margin = 10;
                dockPreviewButton.SetTextureImage(textureSet.AlbedoTexture);
                dockPreviewButton.TooltipText = !string.IsNullOrEmpty(textureSet.Name)
                                                ? textureSet.Name
                                                : $"Texture {i + 1}";

                parentNode.AddChild(dockPreviewButton);


                var currentIndex = i;
                dockPreviewButton.OnSelect = () => {
                    onSelect(currentIndex);
                };
            }
        }
    }

    public static void AddFoliagesPreviewToParent(TerraBrush terraBrush, Node parentNode, Action<int> onSelect, bool useCircleIcon = false) {
        if (terraBrush.Foliages != null) {
            var foliagePreviewPrefab = ResourceLoaderHelper.Load<PackedScene>("res://addons/terrabrush/Components/DockPreviewButton.tscn");

            for (var i = 0; i < terraBrush.Foliages.Count; i++) {
                var foliage = terraBrush.Foliages[i];
                if (foliage.Definition != null) {
                    var dockPreviewButton = foliagePreviewPrefab.Instantiate<DockPreviewButton>();
                    dockPreviewButton.IconType = useCircleIcon ? IconType.Circle : IconType.Square;
                    dockPreviewButton.Margin = 5;

                    if (foliage.Definition?.MeshMaterial == null && foliage.Definition?.AlbedoTextures?.Length == 0 && foliage.Definition.Mesh != null) {
                        dockPreviewButton.LoadResourcePreview(foliage.Definition.Mesh);
                    } else if (foliage.Definition?.MeshMaterial != null) {
                        dockPreviewButton.LoadResourcePreview(foliage.Definition.MeshMaterial);
                    } else if (foliage.Definition?.AlbedoTextures?.Length > 0) {
                        dockPreviewButton.LoadResourcePreview(foliage.Definition.AlbedoTextures[0]);
                    }

                    dockPreviewButton.TooltipText = !string.IsNullOrEmpty(foliage.Definition?.Mesh?.ResourcePath)
                                                    ? Path.GetFileName(foliage.Definition.Mesh.ResourcePath)
                                                    : $"Foliage {i + 1}";

                    parentNode.AddChild(dockPreviewButton);

                    var currentIndex = i;
                    dockPreviewButton.OnSelect = () => {
                        onSelect(currentIndex);
                    };
                }
            }
        }
    }

    public static void AddObjectsPreviewToParent(TerraBrush terraBrush, Node parentNode, Action<int> onSelect, bool useCircleIcon = false) {
        if (terraBrush.Objects != null) {
            var objectPreviewPrefab = ResourceLoaderHelper.Load<PackedScene>("res://addons/terrabrush/Components/DockPreviewButton.tscn");

            for (var i = 0; i < terraBrush.Objects.Count; i++) {
                var objectItem = terraBrush.Objects[i];

                if (objectItem.Definition?.ObjectScenes?.Count > 0 || objectItem.Definition?.LODMeshes?.Count > 0) {
                    var dockPreviewButton = objectPreviewPrefab.Instantiate<DockPreviewButton>();
                    dockPreviewButton.IconType = useCircleIcon ? IconType.Circle : IconType.Square;
                    dockPreviewButton.Margin = 5;

                    var meshResource = (Resource)(objectItem.Definition?.ObjectScenes?.Count > 0 ? objectItem.Definition?.ObjectScenes[0] : objectItem.Definition?.LODMeshes[0].Meshes[0].Mesh);
                    dockPreviewButton.LoadResourcePreview(meshResource);

                    var packedScenes = objectItem.Definition?.ObjectScenes;
                    var tooltipText = $"Object {i + 1}";
                    if (packedScenes?.Count > 0) {
                        var sceneNames = packedScenes
                            .Where(scene => scene != null)
                            .Select(scene => Path.GetFileName(scene.ResourcePath))
                            .Where(name => !string.IsNullOrEmpty(name));
                        var joinedNames = string.Join(", ", sceneNames);
                        if (!string.IsNullOrEmpty(joinedNames)) {
                            tooltipText = joinedNames;
                        }
                    }

                    dockPreviewButton.TooltipText = tooltipText;

                    parentNode.AddChild(dockPreviewButton);

                    var currentIndex = i;
                    dockPreviewButton.OnSelect = () => {
                        onSelect(currentIndex);
                    };
                }
            }
        }
    }

    public static void AddMetaInfoLayersPreviewToParent(TerraBrush terraBrush, Node parentNode, Action<int> onSelect, bool useCircleIcon = false) {
        if (terraBrush.MetaInfoLayers?.Count > 0) {
            var previewPrefab = ResourceLoaderHelper.Load<PackedScene>("res://addons/terrabrush/Components/DockPreviewButton.tscn");

            for (var i = 0; i < terraBrush.MetaInfoLayers.Count; i++) {
                var metaInfoLayer = terraBrush.MetaInfoLayers[i];

                var dockPreviewButton = previewPrefab.Instantiate<DockPreviewButton>();
                dockPreviewButton.IconType = useCircleIcon ? IconType.Circle : IconType.Square;
                dockPreviewButton.Margin = 10;

                var image = Image.CreateEmpty(48, 48, false, Image.Format.Rgba8);
                image.Fill(metaInfoLayer.Color);
                var imageTexture = ImageTexture.CreateFromImage(image);
                dockPreviewButton.SetTextureImage(imageTexture);
                dockPreviewButton.TooltipText = !string.IsNullOrEmpty(metaInfoLayer.Name)
                                                ? metaInfoLayer.Name
                                                : $"MetaInfo {i + 1}";

                parentNode.AddChild(dockPreviewButton);

                var currentIndex = i;
                dockPreviewButton.OnSelect = () => {
                    onSelect(currentIndex);
                };
            }
        }
    }
}
