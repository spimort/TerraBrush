// This file has been adapted from https://github.com/ShuTheWise/MyQem
// Could not find a licence for it...

using System.Numerics;

namespace TerraBrush.qem;

public class Face {
    public Vertex V1, V2, V3;
    public bool Removed;

    public Face(Vertex v1, Vertex v2, Vertex v3) {
        this.V1 = v1;
        this.V2 = v2;
        this.V3 = v3;
        Removed = false;
    }

    public bool Degenerate => V1 == V2 || V1 == V3 || V2 == V3;

    public override string ToString() {
        return $"V1: {V1.Vector3} V2: {V2.Vector3} V3: {V3.Vector3}";
    }

    public bool Equals2(Vertex v1, Vertex v2, Vertex v3) {
        return Share1V(v1) && Share1V(v2) && Share1V(v3);
    }

    private bool Share1V(Vertex v) {
        return v == V2 || v == V2 || v == V3;
    }

    public Vector3 Normal() {
        Vector3 e1 = V2.Vector3 - V1.Vector3;
        Vector3 e2 = V3.Vector3 - V1.Vector3;
        return e1.Cross(e2).Normalize();
    }
}
