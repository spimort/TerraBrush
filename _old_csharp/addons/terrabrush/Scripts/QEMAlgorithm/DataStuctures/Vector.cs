// This file has been adapted from https://github.com/ShuTheWise/MyQem
// Could not find a licence for it...

using System;
using System.Numerics;

namespace TerraBrush.qem;

public static class VectorEx {
    public static bool Less(this Vector3 a, Vector3 b) {
        if (a.X != b.X) {
            return a.X < b.X;
        }

        if (a.Y != b.Y) {
            return a.Y < b.Y;
        }

        return a.Z < b.Z;
    }

    public static Vector3 Cross(this Vector3 a, Vector3 b) {
        return Vector3.Cross(a, b);
    }

    public static Vector3 Normalize(this Vector3 a) {
        return Vector3.Normalize(a);
    }

    public static float Dot(this Vector3 a, Vector3 b) {
        return Vector3.Dot(a,b);
    }
}
