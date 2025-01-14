using System;

namespace TerraBrush;

public class ToolTypeAttribute : Attribute {
    private Type _paintToolType;

    public Type PaintToolType => _paintToolType;

    public ToolTypeAttribute(Type paintToolType) {
        _paintToolType = paintToolType;
    }
}
