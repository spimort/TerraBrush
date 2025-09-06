#include "foliage_resource.h"

using namespace godot;

void FoliageResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_definition"), &FoliageResource::get_definition);
    ClassDB::bind_method(D_METHOD("set_definition", "value"), &FoliageResource::set_definition);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "definition", PROPERTY_HINT_RESOURCE_TYPE, "FoliageDefinitionResource"), "set_definition", "get_definition");
}

FoliageResource::FoliageResource() {
    _definition = Ref<FoliageDefinitionResource>(nullptr);
}

FoliageResource::~FoliageResource() {}

Ref<FoliageDefinitionResource> FoliageResource::get_definition() const {
    return _definition;
}

void FoliageResource::set_definition(const Ref<FoliageDefinitionResource> &value) {
    _definition = value;
}