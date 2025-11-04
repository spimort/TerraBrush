#ifndef META_INFO_LAYER_RESOURCE_H
#define META_INFO_LAYER_RESOURCE_H

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class MetaInfoLayerResource : public Resource {
    GDCLASS(MetaInfoLayerResource, Resource);

private:
    String _name = "";
    Color _color = Color();

protected:
    static void _bind_methods();

public:
    MetaInfoLayerResource();
    ~MetaInfoLayerResource();

    String get_name() const;
    void set_name(const String value);

    Color get_color() const;
    void set_color(const Color value);
};
#endif