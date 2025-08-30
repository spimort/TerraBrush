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
    float _angleOffset;
    int _buttonCount;
    int _startingButtonsIndex;
    DockPreviewButton *_previewDockPreviewOverItem;
    Control *_controlsContainer;
    Label *_menuLabel;
    TextureRect *_pieBackground;
    Panel *_currentAnglePanel;

    int _buttonsRadiusRatio;
    int _buttonsMinRadius;
    String _label;

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