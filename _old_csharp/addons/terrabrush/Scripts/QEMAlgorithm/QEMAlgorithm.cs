// This file has been adapted from https://github.com/ShuTheWise/MyQem
// Could not find a licence for it...

using System.Linq;
using System.Numerics;

namespace TerraBrush.qem;

using System;
using System.Collections.Generic;

public class CustomPriorityQueue<TElement, TPriority> {
    private Comparison<TPriority> _comparison;
    private List<(TElement Item, TPriority Priority)> _elements = new List<(TElement, TPriority)>();

    public int Count => _elements.Count;

    public CustomPriorityQueue(Comparison<TPriority> comparison) {
        _comparison = comparison;
    }

    public void Enqueue(TElement item, TPriority priority) {
        _elements.Add((item, priority));
        _elements.Sort((x, y) => _comparison(x.Priority, y.Priority));
    }

    public TElement Dequeue() {
        var item = _elements[0];
        _elements.RemoveAt(0);
        return item.Item;
    }

    public void Remove(TElement item) {
        _elements.RemoveAll(e => EqualityComparer<TElement>.Default.Equals(e.Item, item));
        _elements.Sort((x, y) => _comparison(x.Priority, y.Priority));
    }
}

public static class QEMAlgorithm {
    private static void AddVertex(Vector3 v, Dictionary<Vector3, Vertex> dic) {
        if (!dic.ContainsKey(v)) {
            dic.Add(v, new Vertex(v));
        }
    }

    private static int CompFloats(float f, float f2) {
        return (f >= f2) ? 1 : -1;
    }

    public static Mesh Simplify(this Mesh originalMesh, int targetCount) {
        // gather distinct vertices
        var vectorVertex = new Dictionary<Vector3, Vertex>(originalMesh.VertexCount);

        foreach (Triangle t in originalMesh.Tris) {
            AddVertex(t.V1, vectorVertex);
            AddVertex(t.V2, vectorVertex);
            AddVertex(t.V3, vectorVertex);
        }

        // accumulate quadric matrices for each vertex based on its faces
        // assign initial quadric
        foreach (Triangle t in originalMesh.Tris) {
            Matrix q = t.Quadric();
            Vertex v1 = vectorVertex[t.V1];
            Vertex v2 = vectorVertex[t.V2];
            Vertex v3 = vectorVertex[t.V3];

            v1.Quadric = v1.Quadric.Add(q);
            v2.Quadric = v2.Quadric.Add(q);
            v3.Quadric = v3.Quadric.Add(q);
        }

        //vertex -> face map
        Dictionary<Vertex, List<Face>> vertexFaces = new Dictionary<Vertex, List<Face>>(originalMesh.VertexCount);
        foreach (Triangle t in originalMesh.Tris) {
            Vertex v1 = vectorVertex[t.V1];
            Vertex v2 = vectorVertex[t.V2];
            Vertex v3 = vectorVertex[t.V3];

            Face f = new Face(v1, v2, v3);

            vertexFaces.AppendEx(v1, f);
            vertexFaces.AppendEx(v2, f);
            vertexFaces.AppendEx(v3, f);
        }

        //gather distinct pairs
        Dictionary<Pair.Key, Pair> pairs = new Dictionary<Pair.Key, Pair>(originalMesh.TrisCount);
        foreach (Triangle t in originalMesh.Tris) {
            Vertex v1 = vectorVertex[t.V1];
            Vertex v2 = vectorVertex[t.V2];
            Vertex v3 = vectorVertex[t.V3];

            pairs.AddPair(v1, v2);
            pairs.AddPair(v2, v3);
            pairs.AddPair(v1, v3);
        }

        Dictionary<Vertex, List<Pair>> vertexPairs = new Dictionary<Vertex, List<Pair>>(originalMesh.VertexCount);

        foreach (KeyValuePair<Pair.Key, Pair> p in pairs)
        {
            vertexPairs.AppendEx(p.Value.A, p.Value);
            vertexPairs.AppendEx(p.Value.B, p.Value);
        }

        var priorityQueue = new CustomPriorityQueue<Pair, float>(CompFloats);
        foreach (KeyValuePair<Pair.Key, Pair> item in pairs) {
            item.Value.Error();
            priorityQueue.Enqueue(item.Value, item.Value.CachedError);
        }

        //take best pair
        int currentFaceCount = originalMesh.Tris.Length;
        int targetFaceCount = targetCount;
        while (currentFaceCount > targetFaceCount && priorityQueue.Count > 0) {
            //best pair
            Pair p = priorityQueue.Dequeue();

            if (p.Removed)
                continue;

            p.Removed = true;

            //get distinct faces
            var distinctFaces = new HashSet<Face>();
            if (vertexFaces.ContainsKey(p.A)) {
                foreach (var f in vertexFaces[p.A])
                {
                    if (!f.Removed)
                    {
                        if (!distinctFaces.Contains(f))
                            distinctFaces.Add(f);
                    }
                }
            }

            if (vertexFaces.ContainsKey(p.B)) {
                foreach (var f in vertexFaces[p.B]) {
                    if (!f.Removed) {
                        if (!distinctFaces.Contains(f)) {
                            distinctFaces.Add(f);
                        }
                    }
                }
            }

            //get related pairs
            var distintPairs = new HashSet<Pair>();
            if (vertexPairs.ContainsKey(p.A)) {
                foreach (var q in vertexPairs[p.A]) {
                    if (!q.Removed) {
                        if (!distintPairs.Contains(q)) {
                            distintPairs.Add(q);
                        }
                    }
                }
            }

            if (vertexPairs.ContainsKey(p.B)) {
                foreach (var q in vertexPairs[p.B]) {
                    if (!q.Removed) {
                        if (!distintPairs.Contains(q)) {
                            distintPairs.Add(q);
                        }
                    }
                }
            }

            //create new vertex
            Vertex v = new Vertex(p.Vector(), p.Quadric());

            //updateFaces
            var newFaces = new List<Face>();
            bool valid = true;
            foreach (var f in distinctFaces) {
                var (v1, v2, v3) = (f.V1, f.V2, f.V3);

                if (v1 == p.A || v1 == p.B)
                    v1 = v;

                if (v2 == p.A || v2 == p.B)
                    v2 = v;

                if (v3 == p.A || v3 == p.B)
                    v3 = v;

                var face = new Face(v1, v2, v3);

                if (face.Degenerate){
                    continue;
                }

                if (face.Normal().Dot(f.Normal()) < 1e-3) {
                    valid = false;
                    break;
                }

                newFaces.Add(face);
            }

            if (!valid) {
                continue;
            }

            if (vertexFaces.ContainsKey(p.A)) {
                vertexFaces.Remove(p.A);
            }

            if (vertexFaces.ContainsKey(p.B)) {
                vertexFaces.Remove(p.B);
            }

            foreach (var f in distinctFaces) {
                f.Removed = true;
                currentFaceCount--;
            }

            foreach (var f in newFaces) {
                currentFaceCount++;
                vertexFaces.AppendEx(f.V1, f);
                vertexFaces.AppendEx(f.V2, f);
                vertexFaces.AppendEx(f.V3, f);
            }

            if (vertexPairs.ContainsKey(p.A)) {
                vertexPairs.Remove(p.A);
            }

            if (vertexPairs.ContainsKey(p.B)) {
                vertexPairs.Remove(p.B);
            }

            var seen = new Dictionary<Vector3, bool>();

            foreach (var q in distintPairs) {
                q.Removed = true;
                priorityQueue.Remove(q);
                var (a, b) = (q.A, q.B);

                if (a == p.A || a == p.B) {
                    a = v;
                }
                if (b == p.A || b == p.B) {
                    b = v;
                }
                if (b == v) {
                    (a, b) = (b, a);
                    // a = v
                }
                if (seen.ContainsKey(b.Vector3) && seen[b.Vector3]) {
                    //ignore duplicates
                    continue;
                }
                if (!seen.ContainsKey(b.Vector3)) {
                    seen.Add(b.Vector3, true);

                } else {
                    seen[b.Vector3] = true;
                }

                var np = new Pair(a, b);
                np.Error();
                priorityQueue.Enqueue(np, np.CachedError);

                vertexPairs.AppendEx(a, np);
                vertexPairs.AppendEx(b, np);
            }
        }

        //gather distinct faces
        var finalDistinctFaces = new HashSet<Face>();
        foreach (var faces in vertexFaces) {
            foreach (var face in faces.Value) {
                if (!face.Removed) {
                    if (!finalDistinctFaces.Contains(face)) {
                        finalDistinctFaces.Add(face);
                    }
                }
            }
        }

        //create final mesh
        Mesh newMesh = new Mesh {
            Tris = finalDistinctFaces.Select(x => new Triangle(x.V1.Vector3, x.V2.Vector3, x.V3.Vector3)).ToArray()
        };

        return newMesh;
    }

    public static void AppendEx<K, T>(this Dictionary<K, List<T>> dic, K key, T newEntry) {
        if (dic.ContainsKey(key)) {
            List<T> value = dic[key];
            value.Add(newEntry);
        } else {
            dic.Add(key, new List<T> { newEntry });
        }
    }

    public static void AddPair(this Dictionary<Pair.Key, Pair> dic, Vertex a, Vertex b) {
        Pair.Key key = Pair.Key.Make(a, b);
        if (!dic.ContainsKey(key)) {
            dic.Add(Pair.Key.Make(a, b), new Pair(a, b));
        }
    }

    public static Godot.Mesh GetGodotLODMesh(Godot.Mesh originalMesh, int targetVertices, Godot.MeshInstance3D meshInstance = null) {
        var surfaceTool = new Godot.SurfaceTool();

        Godot.ArrayMesh resultMesh = null;
        for (var i = 0; i < originalMesh.GetSurfaceCount(); i++) {
            surfaceTool.Begin(Godot.Mesh.PrimitiveType.Triangles);

            var qemMesh = Mesh.CreateMeshFromGodotMeshSurface(originalMesh, i);
            var qemLODMesh = Simplify(qemMesh, targetVertices);
            var vertices = Mesh.GetGodotVerticesForMesh(qemLODMesh);

            for (var vertexIndex = 0; vertexIndex < vertices.Count; vertexIndex += 3) {
                surfaceTool.SetUV(new Godot.Vector2(0, 0));
                surfaceTool.AddVertex(vertices[vertexIndex]);
                surfaceTool.SetUV(new Godot.Vector2(0.5f, 1));
                surfaceTool.AddVertex(vertices[vertexIndex + 1]);
                surfaceTool.SetUV(new Godot.Vector2(1, 0));
                surfaceTool.AddVertex(vertices[vertexIndex + 2]);
            }

            surfaceTool.GenerateNormals();
            surfaceTool.SetMaterial(meshInstance?.GetSurfaceOverrideMaterial(i) ?? originalMesh.SurfaceGetMaterial(i));

            var commitMesh = surfaceTool.Commit(resultMesh);
            resultMesh ??= commitMesh;
        }

        return resultMesh;
    }
}
