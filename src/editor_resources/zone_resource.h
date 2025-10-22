#ifndef ZONE_RESOURCE_H
#define ZONE_RESOURCE_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/image_texture.hpp>

using namespace godot;

class ZoneResource : public Resource {
    GDCLASS(ZoneResource, Resource);

private:
    Ref<Image> _lockTexture = 0;
    Vector2i _zonePosition = Vector2i();
    Ref<Image> _heightMapTexture = nullptr;
    TypedArray<Ref<Image>> _splatmapsTexture = TypedArray<Ref<Image>>();
    TypedArray<Ref<Image>> _foliagesTexture = TypedArray<Ref<Image>>();
    TypedArray<Ref<Image>> _objectsTexture = TypedArray<Ref<Image>>();
    Ref<Image> _waterTexture = nullptr;
    Ref<Image> _snowTexture = nullptr;
    Ref<Image> _metaInfoTexture = nullptr;

protected:
    static void _bind_methods();

public:
    ZoneResource();
    ~ZoneResource();

    Ref<Image> get_lockTexture() const;
    void set_lockTexture(const Ref<Image> &value);

    Vector2i get_zonePosition() const;
    void set_zonePosition(const Vector2i value);

    Ref<Image> get_heightMapTexture() const;
    void set_heightMapTexture(const Ref<Image> &value);

    TypedArray<Ref<Image>> get_splatmapsTexture() const;
    void set_splatmapsTexture(const TypedArray<Ref<Image>> value);

    TypedArray<Ref<Image>> get_foliagesTexture() const;
    void set_foliagesTexture(const TypedArray<Ref<Image>> value);

    TypedArray<Ref<Image>> get_objectsTexture() const;
    void set_objectsTexture(const TypedArray<Ref<Image>> value);

    Ref<Image> get_waterTexture() const;
    void set_waterTexture(const Ref<Image> &value);

    Ref<Image> get_snowTexture() const;
    void set_snowTexture(const Ref<Image> &value);

    Ref<Image> get_metaInfoTexture() const;
    void set_metaInfoTexture(const Ref<Image> &value);
};
#endif