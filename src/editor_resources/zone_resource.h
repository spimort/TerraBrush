#ifndef ZONE_RESOURCE_H
#define ZONE_RESOURCE_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/image.hpp>

using namespace godot;

class ZoneResource : public Resource {
    GDCLASS(ZoneResource, Resource);

private:
    Ref<Image> _lockImage = 0;
    Vector2i _zonePosition = Vector2i();
    Ref<Image> _heightMapImage = nullptr;
    Ref<Image> _colorImage = nullptr;
    TypedArray<Ref<Image>> _splatmapsImage = TypedArray<Ref<Image>>();
    TypedArray<Ref<Image>> _foliagesImage = TypedArray<Ref<Image>>();
    TypedArray<Ref<Image>> _objectsImage = TypedArray<Ref<Image>>();
    Ref<Image> _waterImage = nullptr;
    Ref<Image> _snowImage = nullptr;
    Ref<Image> _metaInfoImage = nullptr;

protected:
    static void _bind_methods();

public:
    ZoneResource();
    ~ZoneResource();

    Ref<Image> get_lockTexture() const;
    void set_lockTexture(const Ref<Image> &value);

    Vector2i get_zonePosition() const;
    void set_zonePosition(const Vector2i value);

    Ref<Image> get_heightMapImage() const;
    void set_heightMapImage(const Ref<Image> &value);

    Ref<Image> get_colorImage() const;
    void set_colorImage(const Ref<Image> &value);

    TypedArray<Ref<Image>> get_splatmapsImage() const;
    void set_splatmapsImage(const TypedArray<Ref<Image>> value);

    TypedArray<Ref<Image>> get_foliagesImage() const;
    void set_foliagesImage(const TypedArray<Ref<Image>> value);

    TypedArray<Ref<Image>> get_objectsImage() const;
    void set_objectsImage(const TypedArray<Ref<Image>> value);

    Ref<Image> get_waterImage() const;
    void set_waterImage(const Ref<Image> &value);

    Ref<Image> get_snowImage() const;
    void set_snowImage(const Ref<Image> &value);

    Ref<Image> get_metaInfoImage() const;
    void set_metaInfoImage(const Ref<Image> &value);
};
#endif