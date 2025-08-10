// This file has been adapted from https://github.com/ShuTheWise/MyQem
// Could not find a licence for it...

using System.Numerics;

namespace TerraBrush.qem;

public record Triangle {
    public Vector3 V1, V2, V3;

    public Triangle(Vector3 v1, Vector3 v2, Vector3 v3) {
        this.V1 = v1;
        this.V2 = v2;
        this.V3 = v3;
    }

    public Matrix Quadric() {
        var n = Normal();

        var x = V1.X;
        var y = V1.Y;
        var z = V1.Z;
        var a = n.X;
        var b = n.Y;
        var c = n.Z;
        var d = -a * x - b * y - c * z;

        return new Matrix(
            a * a, a * b, a * c, a * d,
            a * b, b * b, b * c, b * d,
            a * c, b * c, c * c, c * d,
            a * d, b * d, c * d, d * d
        );
    }

    public Vector3 Normal() {
        var e1 = V2 - V1;
        var e2 = V3 - V2;
        return e1.Cross(e2).Normalize();
    }
}
