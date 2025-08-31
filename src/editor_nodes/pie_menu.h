#ifndef PIE_MENU_H
#define PIE_MENU_H

#include "dock_preview_button.h"

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/panel.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

class PieMenu : public Control {
    GDCLASS(PieMenu, Control);

private:
    float _angleOffset = 0;
    int _buttonCount = 0;
    int _startingButtonsIndex = 0;
    DockPreviewButton *_previewDockPreviewOverItem = nullptr;
    Control *_controlsContainer = nullptr;
    Label *_menuLabel = nullptr;
    TextureRect *_pieBackground = nullptr;
    Panel *_currentAnglePanel = nullptr;

    int _buttonsRadiusRatio = 0;
    int _buttonsMinRadius = 0;
    String _label = "";

    TypedArray<Control> getButtons();
    float getMouseCurrentAngle();
    int getMouseOverItemIndex();
    void updateCurrentAnglePanel();

protected:
    static void _bind_methods();

public:
    PieMenu();
    ~PieMenu();

    void _ready() override;
    void _process(double delta);
    void _gui_input(const Ref<InputEvent> &event) override;

    void set_buttonsRadiusRatio(const int value);
    void set_buttonsMinRadius(const int value);
    void set_label(const String value);

    void updateContent();
};
#endif