#ifndef IMPORT_IMAGE_ROW_H
#define IMPORT_IMAGE_ROW_H

#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/texture_button.hpp>

using namespace godot;

class ImportImageRow : public PanelContainer {
    GDCLASS(ImportImageRow, PanelContainer);

private:
    TextureButton *_textureButton = nullptr;

    String _imageTypeName = "";
    Ref<Texture2D> _imageTexture = nullptr;
    Ref<Texture2D> _originalNormalTexture = nullptr;
    bool _isSelectorOpen = false;

    void onMouseEnter();
    void onMouseExit();
    void onGuiInput(const Ref<InputEvent> &event);
protected:
    static void _bind_methods();

public:
    ImportImageRow();
    ~ImportImageRow();

    void _ready() override;

    void set_imageTypeName(const String value);

    Ref<Texture2D> get_imageTexture() const;
    void set_imageTexture(const Ref<Texture2D> &value);
};
#endif