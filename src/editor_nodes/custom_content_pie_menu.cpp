#include "custom_content_pie_menu.h"

using namespace godot;

void CustomContentPieMenu::_bind_methods() {

}

CustomContentPieMenu::CustomContentPieMenu() {

}

CustomContentPieMenu::~CustomContentPieMenu() {}

void CustomContentPieMenu::_ready() {
    _pieMenu = memnew(PieMenu);
    _pieMenu->set_buttonsRadiusRatio(20);
    add_child(_pieMenu);
}

PieMenu *CustomContentPieMenu::get_pieMenu() const {
    return _pieMenu;
}