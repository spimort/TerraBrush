// This file has been adapted from https://github.com/ShuTheWise/MyQem
// Could not find a licence for it...

using System;
using System.Numerics;

namespace TerraBrush.qem;

public class Pair {
    public Vertex A, B;
    public float CachedError;
    public bool Removed;

    public Pair(Vertex a, Vertex b) {
        if (a.Vector3.Less(b.Vector3))
        {
            (a, b) = (b, a);
        }
        A = a;
        B = b;
        CachedError = -1;
        Removed = false;
    }

    public Vector3 Vector() {
        var q = Quadric();

        if (Math.Abs(q.Determinant()) > 1e-3)
        {
            var v = q.QuadricVector();
            if (!float.IsNaN(v.X) && !float.IsNaN(v.Y) && !float.IsNaN(v.Z))
                return v;
        }

        //cannot compute best vector with matrix
        // look for vest along edge
        int n = 32;
        var a = A.Vector3;
        var b = B.Vector3;
        var bestE = -1d;
        var bestV = new Vector3();

        for (int i = 0; i < n; i++) {
            int frac = i * (1 / n);
            var v = Vector3.Lerp(a, b, frac);
            var e = A.Quadric.QuadricError(v);
            if (bestE < 0 || e < bestE) {
                bestE = e;
                bestV = v;
            }
        }

        return bestV;
    }

    public float Error() {
        if (CachedError < 0) {
            CachedError = Quadric().QuadricError(Vector());
        }

        return CachedError;
    }

    public Matrix Quadric() {
        return A.Quadric.Add(B.Quadric);
    }

    public record Key {
        public Vector3 A, B;
        public Key(Vector3 a, Vector3 b) {
            A = a;
            B = b;
        }

        public static Key Make(Vertex a, Vertex b) {
            if (a.Vector3.Less(b.Vector3)) {
                return new Key(a.Vector3, b.Vector3);
            }

            return new Key(b.Vector3, a.Vector3);
        }
    }
}
