
using System;
using System.Collections.Generic;
using System.IO;
using Godot;

namespace TerraBrush;

public static class CustomContentLoader {
    public static void AddBrushesPreviewToParent(Node parentNode, Action<int> onSelect) {
        var brushPreviewPrefab = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/BrushPreview.tscn");

        var directories = new List<DirAccess>() {DirAccess.Open("res://addons/terrabrush/Assets/Brushes/")};

        var customBrushesDirectory = (string) ProjectSettings.GetSetting(SettingContants.CustomBrushesFolder);
        if (!string.IsNullOrWhiteSpace(customBrushesDirectory) && DirAccess.DirExistsAbsolute(customBrushesDirectory)) {
            directories.Add(DirAccess.Open(customBrushesDirectory));
        }

        var index = 0;
        foreach (var directory in directories) {
            foreach (var file in directory.GetFiles()) {
                if (file.EndsWith(".png", StringComparison.InvariantCultureIgnoreCase)) {
                    var brushImage = ResourceLoader.Load<Texture2D>(Path.Combine(directory.GetCurrentDir(), file));
                    var brushPreview = brushPreviewPrefab.Instantiate<BrushPreview>();
                    parentNode.AddChild(brushPreview);

                    brushPreview.SetTextureImage(brushImage);

                    var currentIndex = index;
                    brushPreview.OnSelect = () => {
                        onSelect(currentIndex);
                    };

                    index += 1;
                }
            }
        }
    }

    public static void AddTexturesPreviewToParent(TerraBrush terraBrush, Node parentNode, Action<int> onSelect) {
        if (terraBrush.TextureSets?.TextureSets != null) {
            var texturePreviewPrefab = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/TexturePreview.tscn");

            for (var i = 0; i < terraBrush.TextureSets.TextureSets.Length; i++) {
                var textureSet = terraBrush.TextureSets.TextureSets[i];

                var texturePreview = texturePreviewPrefab.Instantiate<TexturePreview>();
                parentNode.AddChild(texturePreview);

                texturePreview.SetTextureImage(textureSet.AlbedoTexture);

                var currentIndex = i;
                texturePreview.OnSelect = () => {
                    onSelect(currentIndex);
                };
            }
        }
    }

    public static void AddFoliagesPreviewToParent(TerraBrush terraBrush, Node parentNode, Action<int> onSelect) {
        if (terraBrush.Foliages != null) {
            var foliagePreviewPrefab = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/FoliagePreview.tscn");

            for (var i = 0; i < terraBrush.Foliages.Length; i++) {
                var foliage = terraBrush.Foliages[i];
                if (foliage.Definition != null) {
                    var foliagePreview = foliagePreviewPrefab.Instantiate<FoliagePreview>();
                    parentNode.AddChild(foliagePreview);

                    foliagePreview.LoadPreview(foliage.Definition.Mesh, foliage.Definition?.MeshMaterial);

                    var currentIndex = i;
                    foliagePreview.OnSelect = () => {
                        onSelect(currentIndex);
                    };
                }
            }
        }
    }

    public static void AddObjectsPreviewToParent(TerraBrush terraBrush, Node parentNode, Action<int> onSelect) {
        if (terraBrush.Objects != null) {
            var objectPreviewPrefab = ResourceLoader.Load<PackedScene>("res://addons/terrabrush/Components/ObjectPreview.tscn");

            for (var i = 0; i < terraBrush.Objects.Length; i++) {
                var objectItem = terraBrush.Objects[i];

                if (objectItem.Definition?.ObjectScenes != null) {
                    var objectPreview = objectPreviewPrefab.Instantiate<ObjectPreview>();
                    parentNode.AddChild(objectPreview);

                    objectPreview.LoadPreview(objectItem.Definition?.ObjectScenes[0]);

                    var currentIndex = i;
                    objectPreview.OnSelect = () => {
                        onSelect(currentIndex);
                    };
                }
            }
        }
    }
}
