#ifndef TOOL_INFO_H
#define TOOL_INFO_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/tween.hpp>

using namespace godot;

class ToolInfo : public Control {
    GDCLASS(ToolInfo, Control)

private:
    PanelContainer *_panelContainer;
    Label *_label;
    Ref<Tween> _tween;

protected:
    static void _bind_methods();

public:
    ToolInfo();
    ~ToolInfo();

    void _ready() override;

    void setText(const String text);
};
#endif