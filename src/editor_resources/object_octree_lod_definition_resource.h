#ifndef OBJECT_OCTREE_LOD_DEFINITION_RESOURCE
#define OBJECT_OCTREE_LOD_DEFINITION_RESOURCE

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class ObjectOctreeLODDefinitionResource : public Resource {
    GDCLASS(ObjectOctreeLODDefinitionResource, Resource);

private:
    float _maxDistance = 0;
    bool _addCollision = false;
    bool _automaticLOD = false;
    int _automaticLODTargetVertices = 0;
    int _objectFrequency = 0;

protected:
    static void _bind_methods();

public:
    ObjectOctreeLODDefinitionResource();
    ~ObjectOctreeLODDefinitionResource();

    float get_maxDistance() const;
    void set_maxDistance(const float value);

    bool get_addCollision() const;
    void set_addCollision(const bool value);

    bool get_automaticLOD() const;
    void set_automaticLOD(const bool value);

    int get_automaticLODTargetVertices() const;
    void set_automaticLODTargetVertices(const int value);

    int get_objectFrequency() const;
    void set_objectFrequency(const int value);
};
#endif