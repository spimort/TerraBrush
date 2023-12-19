using System;
using Godot;

namespace TerraBrush;

public interface IDockPreview {
    void SetTextureImage(Texture2D texture);
    void ShowMouseOver();
    void ClearMouseOver();
    Action OnSelect { get;set; }
}
