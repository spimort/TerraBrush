using System;
using System.Collections.Generic;
using System.Linq;
using Godot;

namespace TerraBrush;

[Tool]
public partial class Clipmap : Node3D {
    private ShaderMaterial _clipmapShader;

    [NodePath] private MeshInstance3D _clipmapMesh;

    [Export] public int ZonesSize { get;set; }
    [Export] public ZonesResource TerrainZones { get;set; }
    [Export] public int Levels { get;set; } = 8;
    [Export] public int RowsPerLevel { get;set; } = 21;
    [Export] public float InitialCellWidth { get;set; } = 1;
    [Export] public ShaderMaterial Shader { get;set; }

    public MeshInstance3D ClipmapMesh => _clipmapMesh;

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

        SetNotifyTransform(true);
    }

    public override void _Process(double delta) {
        if (!Engine.IsEditorHint()) {
            var cameraPosition = this.GetViewport()?.GetCamera3D()?.GlobalPosition ?? Vector3.Zero;
            UpdateClipmapMeshPosition(cameraPosition);
        }
    }

    public override void _Notification(int what) {
        base._Notification(what);

        if (what == NotificationTransformChanged) {
            UpdateShaderOffsetPosition();
        }
    }

    public void UpdateEditorCameraPosition(Camera3D viewportCamera) {
        UpdateClipmapMeshPosition(viewportCamera.GlobalPosition);
    }

    private void UpdateClipmapMeshPosition(Vector3 position) {
        var offset = 0.0f;
        var isEven = ZonesSize % 2 == 0;
        if (isEven) {
            offset = InitialCellWidth / 2.0f;
        }

        var xPosition = ((int) Math.Floor(position.X)) + offset;
        var zPosition = ((int) Math.Floor(position.Z)) + offset;

        var maxCellWidth = InitialCellWidth * Mathf.Pow(2, Levels - 1);

        xPosition -= xPosition % maxCellWidth;
        zPosition -= zPosition % maxCellWidth;

        if (isEven) {
            xPosition -= InitialCellWidth / 2.0f;
            zPosition -= InitialCellWidth / 2.0f;
        }

        var newPosition = new Vector3(xPosition, GlobalPosition.Y, zPosition);
        if (newPosition.DistanceTo(_clipmapMesh.GlobalPosition) > maxCellWidth) {
            _clipmapMesh.GlobalPosition = newPosition;
        }
    }

    public void ClearMesh() {
        _clipmapMesh.Mesh = null;
    }

    public void CreateMesh() {
        var clipmapShader = Shader;
        if (clipmapShader == null) {
            clipmapShader = ResourceLoader.Load<ShaderMaterial>("res://addons/terrabrush/Resources/Shaders/clipmap_shader.gdshader");
        }
        _clipmapShader = clipmapShader;
        _clipmapMesh.MaterialOverride = clipmapShader;

        var vertices = new List<Vector3>();
        var uvs = new List<Vector2>();
        var colors = new List<Color>(); // To store information about the zones

        var rowsPerLevel = RowsPerLevel;
        if (rowsPerLevel % 2 == 0) { // The number of rows per level cannot be even
            rowsPerLevel += 1;
        }

        for (var i = 0; i < Levels; i++) {
            GenerateLevel(vertices, uvs, colors, i + 1, rowsPerLevel, InitialCellWidth);
        }

        var arrays = new Godot.Collections.Array();
        arrays.Resize((int) Mesh.ArrayType.Max);
        arrays[(int) Mesh.ArrayType.Vertex] = vertices.ToArray();
        arrays[(int) Mesh.ArrayType.TexUV] = uvs.ToArray();
        arrays[(int) Mesh.ArrayType.Color] = colors.ToArray();

        var normals = new Vector3[vertices.Count];
        Array.Fill(normals, new Vector3(0, 1, 0));
        arrays[(int) Mesh.ArrayType.Normal] = normals;

        var arrayMesh = new ArrayMesh();
        arrayMesh.AddSurfaceFromArrays(Mesh.PrimitiveType.Triangles, arrays);

        _clipmapMesh.Mesh = arrayMesh;
        UpdateAABB();

        clipmapShader.SetShaderParameter(StringNames.HeightmapTextures, TerrainZones.HeightmapTextures);
        clipmapShader.SetShaderParameter(StringNames.ZonesSize, (float) ZonesSize);
        clipmapShader.SetShaderParameter(StringNames.NumberOfZones, (float) TerrainZones.Zones.Length);
		clipmapShader.SetShaderParameter(StringNames.ZonesMap, TerrainZones.ZonesMap);
        UpdateShaderOffsetPosition();
    }

    private void GenerateLevel(List<Vector3> vertices, List<Vector2> uvs, List<Color> colors, int level, int rowsPerLevel, float initialCellWidth) {
        var width = initialCellWidth * ((float) Math.Pow(2, level - 1));

        var startIndex = -1 - rowsPerLevel;
        var toIndex = 0 + rowsPerLevel;

        // The first level is different from the others because it has multiple group of 4 cells
        if (level == 1) {
            startIndex -= 2;
            toIndex += 2;
        }

        // Because the second level is next to the first one which is a strange one, we need to adjust the offset
        var lowerOffsetIndex = 1;
        var upperOffsetIndex = 1;
        if (level == 2) {
            lowerOffsetIndex = 3;
            upperOffsetIndex = -1;
        }

        for (var x = startIndex; x <= toIndex; x++) {
            for (var z = startIndex; z <= toIndex; z++) {
                if (
                    level == 1 ||
                    (x >= startIndex && x < -((rowsPerLevel + lowerOffsetIndex) / 2)) ||
                    (x > ((rowsPerLevel - upperOffsetIndex) / 2) && x <= toIndex) ||
                    (z >= startIndex && z < -((rowsPerLevel + lowerOffsetIndex) / 2)) ||
                    (z > ((rowsPerLevel - upperOffsetIndex) / 2) && z <= toIndex)
                ) {
                    AddSquareVertices(vertices, uvs, x * width, z * width, width);

                    var vertex0MidZone = (x == startIndex && z % 2 != 0) || (z == startIndex && x % 2 != 0);
                    var vertex1MidZone = (x == toIndex && z % 2 != 0) || (z == startIndex && x % 2 == 0);
                    var vertex2MidZone = (x == startIndex && z % 2 == 0) || (z == toIndex && x % 2 != 0);

                    var vertex3MidZone = (x == toIndex && z % 2 != 0) || (z == startIndex && x % 2 == 0);
                    var vertex4MidZone = (x == toIndex && z % 2 == 0) || (z == toIndex && x % 2 == 0);
                    var vertex5MidZone = (x == startIndex && z % 2 == 0) || (z == toIndex && x % 2 != 0);

                    colors.AddRange(new [] {
                        new Color(vertex0MidZone ? 1 : 0, vertex0MidZone && z == startIndex ? 1 : 0, vertex0MidZone && x == startIndex ? 1 : 0, level / 100.0f),
                        new Color(vertex1MidZone ? 1 : 0, vertex1MidZone && z == startIndex ? 1 : 0, vertex1MidZone && x == toIndex ? 1 : 0, level / 100.0f),
                        new Color(vertex2MidZone ? 1 : 0, vertex2MidZone && z == toIndex ? 1 : 0, vertex2MidZone && x == startIndex ? 1 : 0, level / 100.0f),

                        new Color(vertex3MidZone ? 1 : 0, vertex3MidZone && z == startIndex ? 1 : 0, vertex3MidZone && x == toIndex ? 1 : 0, level / 100.0f),
                        new Color(vertex4MidZone ? 1 : 0, vertex4MidZone && z == toIndex ? 1 : 0, vertex4MidZone && x == toIndex ? 1 : 0, level / 100.0f),
                        new Color(vertex5MidZone ? 1 : 0, vertex5MidZone && z == toIndex ? 1 : 0, vertex5MidZone && x == startIndex ? 1 : 0, level / 100.0f),
                    });
                }
            }
        }
    }

    private void AddSquareVertices(List<Vector3> vertices, List<Vector2> uvs, float xPosition, float zPosition, float width) {
        /* Square made of 2 triangles
            2  #  #  #
            |  \  #  #
            |  #  \  #
            0  -  -  1

            5  -  -  4
            #  \  #  |
            #  #  \  |
            #  #  #  3
        */

        vertices.AddRange(new [] {
            new Vector3(xPosition, 0, zPosition),
            new Vector3(xPosition + width, 0, zPosition),
            new Vector3(xPosition, 0, zPosition + width),

            new Vector3(xPosition + width, 0, zPosition),
            new Vector3(xPosition + width, 0, zPosition + width),
            new Vector3(xPosition, 0, zPosition + width)
        });

        uvs.AddRange(new [] {
            new Vector2(0, 1),
            new Vector2(1, 1),
            new Vector2(0, 0),

            new Vector2(1, 1),
            new Vector2(1, 0),
            new Vector2(0, 0)
        });
    }

    public void UpdateAABB() {
        var zonePositions = TerrainZones.Zones.Select(zone => zone.ZonePosition).ToArray();
        var maxX = zonePositions.Max(x => Math.Abs(x.X));
        var maxY = zonePositions.Max(x => Math.Abs(x.Y));

        var aabbXSize = Math.Max(maxX * ZonesSize * 2, ZonesSize * 2);
        var aabbYSize = Math.Max(maxY * ZonesSize * 2, ZonesSize * 2);
        var aabbXPoint = -(aabbXSize / 2);
        var aabbYPoint = -(aabbYSize / 2);
        ((ArrayMesh) _clipmapMesh.Mesh).CustomAabb = new Aabb(new Vector3(aabbXPoint, Math.Max(aabbXPoint, aabbYPoint), aabbYPoint), new Vector3(aabbXSize, Math.Max(aabbXSize, aabbYSize), aabbYSize));
    }

    private void UpdateShaderOffsetPosition() {
        _clipmapShader.SetShaderParameter(StringNames.OffsetPosition, GlobalPosition);
    }
}
