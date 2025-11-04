#ifndef TOOLS_PIE_MENU_H
#define TOOLS_PIE_MENU_H

#include "pie_menu.h"
#include "../terra_brush.h"
#include "../misc/enums.h"

#include <godot_cpp/classes/control.hpp>

using namespace godot;

class ToolsPieMenu : public Control {
    GDCLASS(ToolsPieMenu, Control);

private:
    PieMenu *_pieMenu = nullptr;

    Callable _onToolSelected = Callable();

    void onToolSelected(const TerrainToolType toolType);

protected:
    static void _bind_methods();

public:
    ToolsPieMenu();
    ~ToolsPieMenu();

    void _ready() override;

    void set_onToolSelected(const Callable value);
};
#endif