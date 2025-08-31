#ifndef OBJECT_RESOURCE_H
#define OBJECT_RESOURCE_H

#include "object_definition_resource.h"

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class ObjectResource : public Resource {
    GDCLASS(ObjectResource, Resource);

private:
    Ref<ObjectDefinitionResource> _definition = nullptr;
    bool _hide = false;

protected:
    static void _bind_methods();

public:
    ObjectResource();
    ~ObjectResource();

    Ref<ObjectDefinitionResource> get_definition() const;
    void set_definition(const Ref<ObjectDefinitionResource> &value);

    bool get_hide() const;
    void set_hide(const bool value);
};
#endif