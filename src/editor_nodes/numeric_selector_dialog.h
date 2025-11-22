#ifndef NUMERIC_SELECTOR_DIALOG_H
#define NUMERIC_SELECTOR_DIALOG_H

#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/spin_box.hpp>

#include <functional>

using namespace godot;

class NumericSelectorDialog : public Window {
    GDCLASS(NumericSelectorDialog, Window);

private:
    SpinBox *_spinBox = nullptr;

    float _minValue = std::numeric_limits<float>::min();
    float _maxValue = std::numeric_limits<float>::max();
    std::function<void(float)> _onAccept = nullptr;

    void sendValue();
    void onOkButton();
    void onCancelButton();

protected:
    static void _bind_methods();

public:
    NumericSelectorDialog();
    ~NumericSelectorDialog();

    void _ready() override;
    void _unhandled_input(const Ref<InputEvent> &event) override;

    void init(float minValue, float maxValue, std::function<void(float)> onAccept);
    void setValue(float value);
};
#endif