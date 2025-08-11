#include "object_resource.h"

using namespace godot;

void ObjectResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_definition"), &ObjectResource::get_definition);
    ClassDB::bind_method(D_METHOD("set_definition", "value"), &ObjectResource::set_definition);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "definition", PROPERTY_HINT_RESOURCE_TYPE, "ObjectDefinitionResource"), "set_definition", "get_definition");

    ClassDB::bind_method(D_METHOD("get_hide"), &ObjectResource::get_hide);
    ClassDB::bind_method(D_METHOD("set_hide", "value"), &ObjectResource::set_hide);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "hide"), "set_hide", "get_hide");
}

ObjectResource::ObjectResource() {

}

ObjectResource::~ObjectResource() {}

Ref<ObjectDefinitionResource> ObjectResource::get_definition() const {
    return _definition;
}
void ObjectResource::set_definition(const Ref<ObjectDefinitionResource> &value) {
    _definition = value;
}

bool ObjectResource::get_hide() const {
    return _hide;
}
void ObjectResource::set_hide(const bool value) {
    _hide = value;
}