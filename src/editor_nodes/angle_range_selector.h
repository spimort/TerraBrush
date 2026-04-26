#ifndef ANGLE_RANGE_SELECTOR_H
#define ANGLE_RANGE_SELECTOR_H

#include <godot_cpp/classes/range.hpp>
#include <godot_cpp/classes/label.hpp>

using namespace godot;

class AngleRangeSelector : public Range {
    GDCLASS(AngleRangeSelector, Range);

private:
    enum AngleRangeHandle {
        None = 1,
        Left = 2,
        Right = 3
    };

    const int RectHeight = 4;
    const int RectRadius = 5;
    const int HandleHeight = 12;
    const int ControlOffset = HandleHeight;
    const int LabelWidth = 50;
    const int LabelHeight = 14;
    const int LabelHorizontalOffset = LabelWidth / 2.0;
    const int LabelVerticalOffset = 2;

    Vector2 _rangeValue = Vector2(0, 90);
    AngleRangeHandle _grabbedHandle = AngleRangeHandle::None;
    AngleRangeHandle _hoverHandle = AngleRangeHandle::None;
    Label *_lowValueLabel = nullptr;
    Label *_highValueLabel = nullptr;

    void draw();
    AngleRangeHandle getHandleForMousePosition(Vector2 position) const;
    void setGrabbedHandle(AngleRangeHandle handle);
    void setHoverHandle(AngleRangeHandle handle);
    void updateLabels();
    float getLowValuePosition() const;
    float getHighValuePosition() const;

protected:
    static void _bind_methods();
    void _notification(const int what);

public:
    AngleRangeSelector();
    ~AngleRangeSelector();

    void _ready() override;
    void _gui_input(const Ref<InputEvent> &event) override;

    Vector2 get_rangeValue() const;
    void set_rangeValue(const Vector2 value);
};
#endif