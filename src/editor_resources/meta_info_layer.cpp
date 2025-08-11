#include "meta_info_layer.h"

using namespace godot;

void MetaInfoLayer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_name"), &MetaInfoLayer::get_name);
    ClassDB::bind_method(D_METHOD("set_name", "value"), &MetaInfoLayer::set_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");

    ClassDB::bind_method(D_METHOD("get_color"), &MetaInfoLayer::get_color);
    ClassDB::bind_method(D_METHOD("set_color", "value"), &MetaInfoLayer::set_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
}

MetaInfoLayer::MetaInfoLayer() {}

MetaInfoLayer::~MetaInfoLayer() {}

String MetaInfoLayer::get_name() const {
    return _name;
}
void MetaInfoLayer::set_name(const String value) {
    _name = value;
}

Color MetaInfoLayer::get_color() const {
    return _color;
}
void MetaInfoLayer::set_color(const Color value) {
    _color = value;
}