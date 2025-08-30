#ifndef CUSTOM_CONTENT_PIE_MENU_H
#define CUSTOM_CONTENT_PIE_MENU_H

#include "pie_menu.h"

#include <godot_cpp/classes/control.hpp>

using namespace godot;

class CustomContentPieMenu : public Control {
    GDCLASS(CustomContentPieMenu, Control);

private:
    PieMenu *_pieMenu;

protected:
    static void _bind_methods();

public:
    CustomContentPieMenu();
    ~CustomContentPieMenu();

    void _ready() override;

    PieMenu *get_pieMenu() const;
};
#endif