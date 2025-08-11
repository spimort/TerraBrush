#include "meta_info_layer_resource.h"

using namespace godot;

void MetaInfoLayerResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_name"), &MetaInfoLayerResource::get_name);
    ClassDB::bind_method(D_METHOD("set_name", "value"), &MetaInfoLayerResource::set_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");

    ClassDB::bind_method(D_METHOD("get_color"), &MetaInfoLayerResource::get_color);
    ClassDB::bind_method(D_METHOD("set_color", "value"), &MetaInfoLayerResource::set_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
}

MetaInfoLayerResource::MetaInfoLayerResource() {}

MetaInfoLayerResource::~MetaInfoLayerResource() {}

String MetaInfoLayerResource::get_name() const {
    return _name;
}
void MetaInfoLayerResource::set_name(const String value) {
    _name = value;
}

Color MetaInfoLayerResource::get_color() const {
    return _color;
}
void MetaInfoLayerResource::set_color(const Color value) {
    _color = value;
}