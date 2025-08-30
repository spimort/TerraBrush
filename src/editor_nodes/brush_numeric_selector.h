#ifndef BRUSH_NUMERIC_SELECTOR_H
#define BRUSH_NUMERIC_SELECTOR_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

class BrushNumericSelector : public Control {
    GDCLASS(BrushNumericSelector, Control);

private:
    const int BackgroundMargin = 10;

    TextureRect *_background;
    TextureRect *_brushPreview;
    Label *_valueLabel;

    int _brushSizeFactor;
    Color _widgetColor;
    int _minValue;
    int _maxValue;

    Callable _onValueSelected;
    Callable _onCancel;

    int getMouseDistance();

protected:
    static void _bind_methods();

public:
    BrushNumericSelector();
    ~BrushNumericSelector();

    void _ready() override;
    void _process(double delta) override;
    void _gui_input(const Ref<InputEvent> &event) override;

    void set_brushSizeFactor(const int value);
    void set_widgetColor(const Color value);
    void set_minValue(const int value);
    void set_maxValue(const int value);

    void set_onValueSelected(const Callable value);
    void set_onCancel(const Callable value);

    void setInitialValue(float value);
    void updateValue(float value);
    void requestSelectValue();
};
#endif