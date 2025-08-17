#ifndef DOCK_PREVIEW_BUTTON_H
#define DOCK_PREVIEW_BUTTON_H

#include <godot_cpp/classes/texture_button.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/label.hpp>

using namespace godot;

class DockPreviewButton : public TextureButton {
    GDCLASS(DockPreviewButton, TextureButton)

public:
    enum IconType {
        Square = 1,
        Circle = 2,
        CircleWithSign = 3
    };

private:
    Color _normalColor;
    Color _hoverColor;
    Color _pressedColor;
    MarginContainer *_marginContainer;
    TextureRect *_textureRect;
    Label *_label;
    Ref<Texture2D> _textureSquare;
    Ref<Texture2D> _textureCircle;
    Ref<Texture2D> _textureCircleWithSign;

    Ref<Texture2D> _buttonImage;
    IconType _iconType;
    int _margin;
    String _text;
    Callable _onSelect;

    void calculateTextures();
    void setTextureColor(DrawMode drawMode);
    void onPreviewThumbnailReady(const String path, const Ref<Texture2D> &preview,  const Ref<Texture2D> &thumbnailPreview, const String resourceName);

protected:
    static void _bind_methods();
    void _notification(const int what);

public:
    DockPreviewButton();
    ~DockPreviewButton();

    void _ready() override;

    Ref<Texture2D> get_buttonImage() const;
    void set_buttonImage(const Ref<Texture2D> &value);

    void set_iconType(const IconType value);
    void set_margin(const int value);
    void set_text(const String value);
    void set_onSelect(const Callable value);

    void onItemSelect();
    void setTextureImage(const Ref<Texture2D> &toolImage);
    void showMouseOver();
    void clearMouseOver();
    void loadResourcePreview(const Ref<Resource> &resource);
};
#endif