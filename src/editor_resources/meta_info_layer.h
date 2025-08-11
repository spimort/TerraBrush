#ifndef META_INFO_LAYER_H
#define META_INFO_LAYER_H

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class MetaInfoLayer : public Resource {
    GDCLASS(MetaInfoLayer, Resource)

private:
    String _name;
    Color _color;

protected:
    static void _bind_methods();

public:
    MetaInfoLayer();
    ~MetaInfoLayer();

    String get_name() const;
    void set_name(const String value);

    Color get_color() const;
    void set_color(const Color value);
};
#endif