// This file has been adapted from https://github.com/ShuTheWise/MyQem
// Could not find a licence for it...

using System.Collections.Generic;
using System.Linq;
using System.Numerics;

namespace TerraBrush.qem;

public struct Mesh {
    public Triangle[] Tris;

    public readonly int TrisCount => Tris.Length;
    public readonly int VertexCount => Tris.SelectMany(x => new[] { x.V1, x.V2, x.V3 }).Distinct().Count();

    public static Mesh CreateMeshFromGodotMesh(Godot.Mesh mesh) {
        var allSurfacesTriangles = new List<Triangle>();
        for (var i = 0; i < mesh.GetSurfaceCount(); i++) {
            allSurfacesTriangles.AddRange(GetTrianglesFromGodotMesh(mesh, i));
        }

        var resultMesh = new Mesh();
        resultMesh.Tris = allSurfacesTriangles.ToArray();

        return resultMesh;
    }

    public static Mesh CreateMeshFromGodotMeshSurface(Godot.Mesh mesh, int surface) {
        var triangles = GetTrianglesFromGodotMesh(mesh, surface);

        var resultMesh = new Mesh();
        resultMesh.Tris = triangles.ToArray();

        return resultMesh;
    }

    public static List<Triangle> GetTrianglesFromGodotMesh(Godot.Mesh mesh, int surface) {
        var triangles = new List<Triangle>();
        var surfaceArrays = mesh.SurfaceGetArrays(surface);
        var vertices = (Godot.Collections.Array<Godot.Vector3>) surfaceArrays[(int) Godot.Mesh.ArrayType.Vertex];
        var arrays = (Godot.Collections.Array<int>) surfaceArrays[(int) Godot.Mesh.ArrayType.Index];

        for (var indiceIndex = 0; indiceIndex < arrays.Count; indiceIndex += 3) {
            var vertex1 = vertices[arrays[indiceIndex]];
            var vertex2 = vertices[arrays[indiceIndex + 1]];
            var vertex3 = vertices[arrays[indiceIndex + 2]];
            Vector3 v1 = new(vertex1.X, vertex1.Y, vertex1.Z);
            Vector3 v2 = new(vertex2.X, vertex2.Y, vertex2.Z);
            Vector3 v3 = new(vertex3.X, vertex3.Y, vertex3.Z);

            triangles.Add(new Triangle(v1, v2, v3));
        }

        return triangles;
    }

    public static List<Godot.Vector3> GetGodotVerticesForMesh(Mesh mesh) {
        var vertices = mesh.Tris.SelectMany(triangle => {
            return new List<Godot.Vector3>() {
                new Godot.Vector3(triangle.V1.X, triangle.V1.Y, triangle.V1.Z),
                new Godot.Vector3(triangle.V2.X, triangle.V2.Y, triangle.V2.Z),
                new Godot.Vector3(triangle.V3.X, triangle.V3.Y, triangle.V3.Z),
            };
        }).ToList();

        return vertices;
    }

    public static Godot.ArrayMesh CreateGodotMeshForMesh(Mesh mesh) {
        var vertices = GetGodotVerticesForMesh(mesh);

        var surfaceTool = new Godot.SurfaceTool();
        surfaceTool.Begin(Godot.Mesh.PrimitiveType.Triangles);

        for (var i = 0; i < vertices.Count(); i += 3) {
            surfaceTool.AddVertex(vertices[i]);
            surfaceTool.AddVertex(vertices[i + 1]);
            surfaceTool.AddVertex(vertices[i + 2]);
        }

        return surfaceTool.Commit();
    }
}
