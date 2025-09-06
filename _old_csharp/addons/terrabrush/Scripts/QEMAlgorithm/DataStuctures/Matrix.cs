// This file has been adapted from https://github.com/ShuTheWise/MyQem
// Could not find a licence for it...

using System.Numerics;

namespace TerraBrush.qem;

public struct Matrix {
    public float x00, x01, x02, x03;
    public float x10, x11, x12, x13;
    public float x20, x21, x22, x23;
    public float x30, x31, x32, x33;

    #region Ctor
    public Matrix(
        float x00,
        float x01,
        float x02,
        float x03,
        float x10,
        float x11,
        float x12,
        float x13,
        float x20,
        float x21,
        float x22,
        float x23,
        float x30,
        float x31,
        float x32,
        float x33
    ) {
        this.x00 = x00;
        this.x01 = x01;
        this.x02 = x02;
        this.x03 = x03;
        this.x10 = x10;
        this.x11 = x11;
        this.x12 = x12;
        this.x13 = x13;
        this.x20 = x20;
        this.x21 = x21;
        this.x22 = x22;
        this.x23 = x23;
        this.x30 = x30;
        this.x31 = x31;
        this.x32 = x32;
        this.x33 = x33;
    }
    #endregion
}

public static class MatrixEx {
    public static float QuadricError(this Matrix a, Vector3 v) {
        return (v.X * a.x00 * v.X + v.Y * a.x10 * v.X + v.Z * a.x20 * v.X + a.x30 * v.X +
            v.X * a.x01 * v.Y + v.Y * a.x11 * v.Y + v.Z * a.x21 * v.Y + a.x31 * v.Y +
            v.X * a.x02 * v.Z + v.Y * a.x12 * v.Z + v.Z * a.x22 * v.Z + a.x32 * v.Z +
            v.X * a.x03 + v.Y * a.x13 + v.Z * a.x23 + a.x33);
    }

    public static Vector3 QuadricVector(this Matrix a) {
        var m = new Matrix(
            a.x00, a.x01, a.x02, a.x03,
            a.x10, a.x11, a.x12, a.x13,
            a.x20, a.x21, a.x22, a.x23,
            0, 0, 0, 1);

        return m.Inverse().MulPosition(new Vector3());
    }

    public static Matrix Add(this Matrix a, Matrix b) {
        return new Matrix(
            a.x00 + b.x00, a.x10 + b.x10, a.x20 + b.x20, a.x30 + b.x30,
            a.x01 + b.x01, a.x11 + b.x11, a.x21 + b.x21, a.x31 + b.x31,
            a.x02 + b.x02, a.x12 + b.x12, a.x22 + b.x22, a.x32 + b.x32,
            a.x03 + b.x03, a.x13 + b.x13, a.x23 + b.x23, a.x33 + b.x33
        );
    }

    public static Matrix Inverse(this Matrix a) {
        Matrix m;
        var r = 1 / Determinant(a);
        m.x00 = (a.x12 * a.x23 * a.x31 - a.x13 * a.x22 * a.x31 + a.x13 * a.x21 * a.x32 - a.x11 * a.x23 * a.x32 - a.x12 * a.x21 * a.x33 + a.x11 * a.x22 * a.x33) * r;
        m.x01 = (a.x03 * a.x22 * a.x31 - a.x02 * a.x23 * a.x31 - a.x03 * a.x21 * a.x32 + a.x01 * a.x23 * a.x32 + a.x02 * a.x21 * a.x33 - a.x01 * a.x22 * a.x33) * r;
        m.x02 = (a.x02 * a.x13 * a.x31 - a.x03 * a.x12 * a.x31 + a.x03 * a.x11 * a.x32 - a.x01 * a.x13 * a.x32 - a.x02 * a.x11 * a.x33 + a.x01 * a.x12 * a.x33) * r;
        m.x03 = (a.x03 * a.x12 * a.x21 - a.x02 * a.x13 * a.x21 - a.x03 * a.x11 * a.x22 + a.x01 * a.x13 * a.x22 + a.x02 * a.x11 * a.x23 - a.x01 * a.x12 * a.x23) * r;
        m.x10 = (a.x13 * a.x22 * a.x30 - a.x12 * a.x23 * a.x30 - a.x13 * a.x20 * a.x32 + a.x10 * a.x23 * a.x32 + a.x12 * a.x20 * a.x33 - a.x10 * a.x22 * a.x33) * r;
        m.x11 = (a.x02 * a.x23 * a.x30 - a.x03 * a.x22 * a.x30 + a.x03 * a.x20 * a.x32 - a.x00 * a.x23 * a.x32 - a.x02 * a.x20 * a.x33 + a.x00 * a.x22 * a.x33) * r;
        m.x12 = (a.x03 * a.x12 * a.x30 - a.x02 * a.x13 * a.x30 - a.x03 * a.x10 * a.x32 + a.x00 * a.x13 * a.x32 + a.x02 * a.x10 * a.x33 - a.x00 * a.x12 * a.x33) * r;
        m.x13 = (a.x02 * a.x13 * a.x20 - a.x03 * a.x12 * a.x20 + a.x03 * a.x10 * a.x22 - a.x00 * a.x13 * a.x22 - a.x02 * a.x10 * a.x23 + a.x00 * a.x12 * a.x23) * r;
        m.x20 = (a.x11 * a.x23 * a.x30 - a.x13 * a.x21 * a.x30 + a.x13 * a.x20 * a.x31 - a.x10 * a.x23 * a.x31 - a.x11 * a.x20 * a.x33 + a.x10 * a.x21 * a.x33) * r;
        m.x21 = (a.x03 * a.x21 * a.x30 - a.x01 * a.x23 * a.x30 - a.x03 * a.x20 * a.x31 + a.x00 * a.x23 * a.x31 + a.x01 * a.x20 * a.x33 - a.x00 * a.x21 * a.x33) * r;
        m.x22 = (a.x01 * a.x13 * a.x30 - a.x03 * a.x11 * a.x30 + a.x03 * a.x10 * a.x31 - a.x00 * a.x13 * a.x31 - a.x01 * a.x10 * a.x33 + a.x00 * a.x11 * a.x33) * r;
        m.x23 = (a.x03 * a.x11 * a.x20 - a.x01 * a.x13 * a.x20 - a.x03 * a.x10 * a.x21 + a.x00 * a.x13 * a.x21 + a.x01 * a.x10 * a.x23 - a.x00 * a.x11 * a.x23) * r;
        m.x30 = (a.x12 * a.x21 * a.x30 - a.x11 * a.x22 * a.x30 - a.x12 * a.x20 * a.x31 + a.x10 * a.x22 * a.x31 + a.x11 * a.x20 * a.x32 - a.x10 * a.x21 * a.x32) * r;
        m.x31 = (a.x01 * a.x22 * a.x30 - a.x02 * a.x21 * a.x30 + a.x02 * a.x20 * a.x31 - a.x00 * a.x22 * a.x31 - a.x01 * a.x20 * a.x32 + a.x00 * a.x21 * a.x32) * r;
        m.x32 = (a.x02 * a.x11 * a.x30 - a.x01 * a.x12 * a.x30 - a.x02 * a.x10 * a.x31 + a.x00 * a.x12 * a.x31 + a.x01 * a.x10 * a.x32 - a.x00 * a.x11 * a.x32) * r;
        m.x33 = (a.x01 * a.x12 * a.x20 - a.x02 * a.x11 * a.x20 + a.x02 * a.x10 * a.x21 - a.x00 * a.x12 * a.x21 - a.x01 * a.x10 * a.x22 + a.x00 * a.x11 * a.x22) * r;
        return m;
    }

    public static float Determinant(this Matrix a) {
        return (a.x00 * a.x11 * a.x22 * a.x33 - a.x00 * a.x11 * a.x23 * a.x32 +
            a.x00 * a.x12 * a.x23 * a.x31 - a.x00 * a.x12 * a.x21 * a.x33 +
            a.x00 * a.x13 * a.x21 * a.x32 - a.x00 * a.x13 * a.x22 * a.x31 -
            a.x01 * a.x12 * a.x23 * a.x30 + a.x01 * a.x12 * a.x20 * a.x33 -
            a.x01 * a.x13 * a.x20 * a.x32 + a.x01 * a.x13 * a.x22 * a.x30 -
            a.x01 * a.x10 * a.x22 * a.x33 + a.x01 * a.x10 * a.x23 * a.x32 +
            a.x02 * a.x13 * a.x20 * a.x31 - a.x02 * a.x13 * a.x21 * a.x30 +
            a.x02 * a.x10 * a.x21 * a.x33 - a.x02 * a.x10 * a.x23 * a.x31 +
            a.x02 * a.x11 * a.x23 * a.x30 - a.x02 * a.x11 * a.x20 * a.x33 -
            a.x03 * a.x10 * a.x21 * a.x32 + a.x03 * a.x10 * a.x22 * a.x31 -
            a.x03 * a.x11 * a.x22 * a.x30 + a.x03 * a.x11 * a.x20 * a.x32 -
            a.x03 * a.x12 * a.x20 * a.x31 + a.x03 * a.x12 * a.x21 * a.x30);
    }

    public static Vector3 MulPosition(this Matrix a, Vector3 b) {
        return new Vector3(
            a.x00 * b.X + a.x01 * b.Y + a.x02 * b.Z + a.x03,
            a.x10 * b.X + a.x11 * b.Y + a.x12 * b.Z + a.x13,
            a.x20 * b.X + a.x21 * b.Y + a.x22 * b.Z + a.x23);
    }
}
