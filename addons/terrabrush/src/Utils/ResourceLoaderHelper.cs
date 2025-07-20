using Godot;

namespace TerraBrush;

public static class ResourceLoaderHelper {
    public static T Load<T>(string path) where T : Resource {
        return (T)ResourceLoader.Singleton.Load(path);
    }
}