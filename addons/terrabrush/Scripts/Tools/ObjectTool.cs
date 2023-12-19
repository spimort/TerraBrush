using System;
using System.Linq;
using Godot;

namespace TerraBrush;

public class ObjectTool : ToolBase {
    public override void Paint(TerraBrush terraBrush, TerrainToolType toolType, Image brushImage, int brushSize, float brushStrength, Vector2 imagePosition) {
        if (terraBrush.ObjectIndex == null) {
            return;
        }

        var currentObject = terraBrush.Objects[terraBrush.ObjectIndex.Value];
        if (currentObject?.Definition?.ObjectScenes == null) {
            return;
        }

        var currentObjectImage = currentObject.Texture.GetImage();
        var heightMapImage = terraBrush.HeightMap.GetImage();
        var waterImage = terraBrush.WaterTexture?.GetImage();

        var currentObjectsNode = terraBrush.ObjectsContainerNode.GetNode<Node3D>(terraBrush.ObjectIndex.ToString());

        var noiseTexture = currentObject.Definition?.NoiseTexture ?? terraBrush.DefaultNoise;
        Image noiseImage = null;
        if (noiseTexture != null) {
            noiseImage = noiseTexture.GetImage();
        }

        ForEachBrushPixel(terraBrush, brushImage, brushSize, imagePosition, (pixelBrushStrength, xPosition, yPosition) => {
            var randomItemIndex = Utils.GetNextIntWithSeed((xPosition * 1000) + yPosition, 0, currentObject.Definition.ObjectScenes.Count() - 1);

            var currentPixel = currentObjectImage.GetPixel(xPosition, yPosition);
            var newColor = currentPixel;

            if (pixelBrushStrength > 0f) {
                var nodeName = $"{xPosition}_{yPosition}";

                if (toolType == TerrainToolType.ObjectAdd) {
                    var objectFrequency = currentObject.Definition.ObjectFrequency < 1 ? terraBrush.DefaultObjectFrequency : currentObject.Definition.ObjectFrequency;

                    if (xPosition % objectFrequency == 0 && yPosition % objectFrequency == 0) {
                        var existingNode = currentObjectsNode.GetNodeOrNull<Node3D>(nodeName);
                        if (existingNode == null) {
                            var resultPosition = new Vector3(xPosition, 0, yPosition);
                            if (noiseImage != null) {
                                var noisePixel = noiseImage.GetPixel(xPosition, yPosition).R;
                                var randomValueX = Utils.GetNextFloatWithSeed((int) (noisePixel * 100), -currentObject.Definition.RandomRange, currentObject.Definition.RandomRange);
                                var randomValueZ = Utils.GetNextFloatWithSeed(1 + (int) (noisePixel * 100), -currentObject.Definition.RandomRange, currentObject.Definition.RandomRange);
                                resultPosition += new Vector3(randomValueX, 0, randomValueZ);
                            }

                            var resultImagePosition = new Vector2I((int) Math.Round(resultPosition.X), (int) Math.Round(resultPosition.Z));
                            if (resultImagePosition.X >= 0 && resultImagePosition.X < terraBrush.TerrainSize && resultImagePosition.Y >= 0 && resultImagePosition.Y < terraBrush.TerrainSize) {
                                var newNode = currentObject.Definition.ObjectScenes[randomItemIndex].Instantiate<Node3D>();
                                newNode.Name = nodeName;
                                currentObjectsNode.AddChild(newNode);

                                var heightmapPixel = heightMapImage.GetPixel(resultImagePosition.X, resultImagePosition.Y);
                                var waterHeight = waterImage?.GetPixel(resultImagePosition.X, resultImagePosition.Y).R ?? 0;
                                resultPosition -= new Vector3(terraBrush.TerrainSize / 2, -((heightmapPixel.R * TerraBrush.HeightMapFactor) - (waterHeight * (terraBrush.WaterDefinition?.WaterFactor ?? 0))), terraBrush.TerrainSize / 2);

                                newNode.Position = resultPosition;

                                if (currentObject.Definition.RandomYRotation) {
                                    newNode.RotationDegrees = new Vector3(newNode.RotationDegrees.X, Utils.GetNextFloatWithSeed((xPosition * 1000) + yPosition, 0f, 360f), newNode.RotationDegrees.Z);
                                }
                            }
                        }

                        newColor = Colors.Red;
                    }
                } else {
                    var existingNode = currentObjectsNode.GetNodeOrNull<Node3D>(nodeName);
                    if (existingNode != null) {
                        existingNode.QueueFree();
                    }

                    newColor = Colors.Transparent;
                }

                currentObjectImage.SetPixel(xPosition, yPosition, newColor);
            }
        });
    }
}
