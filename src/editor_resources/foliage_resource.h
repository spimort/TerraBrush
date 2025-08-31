#ifndef FOLIAGE_RESOURCE_H
#define FOLIAGE_RESOURCE_H

#include "foliage_definition_resource.h"

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class FoliageResource : public Resource {
    GDCLASS(FoliageResource, Resource);

private:
    Ref<FoliageDefinitionResource> _definition = nullptr;

protected:
    static void _bind_methods();

public:
    FoliageResource();
    ~FoliageResource();

    Ref<FoliageDefinitionResource> get_definition() const;
    void set_definition(const Ref<FoliageDefinitionResource> &value);
};
#endif