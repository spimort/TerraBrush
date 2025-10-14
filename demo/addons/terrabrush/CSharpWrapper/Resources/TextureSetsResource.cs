using System.Linq;
using Godot;

namespace TerraBrush;

public partial class TextureSetsResource : Resource {
    private Variant _godotHandle;

    public TextureSetResource[] TextureSets {
        get {
            return _godotHandle.AsGodotObject().Call("get_textureSets").AsGodotArray<Variant>()?.Select(x => new TextureSetResource(x)).ToArray();
        }
        set {
            _godotHandle.AsGodotObject().Call("set_textureSets", value == null ? null : new Godot.Collections.Array(value.Select(x => x.GodotHandle())));
        }
    }

    public static implicit operator TextureSetsResource(Variant handle) => new(handle);

    public TextureSetsResource(Variant handle) {
        _godotHandle = handle;
    }

    public TextureSetsResource() {
        _godotHandle = ClassDB.Instantiate("TextureSetsResource");
    }

    internal Variant GodotHandle() {
        return _godotHandle;
    }

    public new string GetPath() => _godotHandle.AsGodotObject().Call("get_path").AsString();
}
