using System.Diagnostics.CodeAnalysis;

namespace TerraBrush;

public class ToolTypeAttribute : Attribute {
    [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)]
    private Type _paintToolType;

    [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)]
    public Type PaintToolType => _paintToolType;

    public ToolTypeAttribute([DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] Type paintToolType) {
        _paintToolType = paintToolType;
    }
}
