#ifndef TOOLS_PIE_MENU_H
#define TOOLS_PIE_MENU_H

#include "pie_menu.h"
#include "../terra_brush.h"

#include <godot_cpp/classes/control.hpp>

using namespace godot;

class ToolsPieMenu : public Control {
    GDCLASS(ToolsPieMenu, Control)

private:
    PieMenu *_pieMenu;

    Callable _onToolSelected;

    void onToolSelected(const TerraBrush::TerrainToolType toolType);

protected:
    static void _bind_methods();

public:
    ToolsPieMenu();
    ~ToolsPieMenu();

    void _ready() override;

    void set_onToolSelected(const Callable value);
};
#endif