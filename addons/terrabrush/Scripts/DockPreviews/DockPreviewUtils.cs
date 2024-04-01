#if TOOLS
using Godot;

public static class DockPreviewUtils {
    public static void HandleCircleIconShader(TextureButton parentButton, TextureRect iconTextureRect) {
        var shader = ResourceLoader.Load<Shader>("res://addons/terrabrush/Resources/Shaders/circle_mask_shader.gdshader");
        parentButton.Material = new ShaderMaterial() {
            Shader = shader
        };

        iconTextureRect.Material = new ShaderMaterial() {
            Shader = shader
        };
        ((ShaderMaterial)iconTextureRect.Material).SetShaderParameter("CircleSize", 0.4);
    }
}
#endif
