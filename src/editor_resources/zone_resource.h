#ifndef ZONE_RESOURCE_H
#define ZONE_RESOURCE_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/image_texture.hpp>

using namespace godot;

class ZoneResource : public Resource {
    GDCLASS(ZoneResource, Resource);

private:
    Ref<ImageTexture> _lockTexture;
    Vector2i _zonePosition;
    Ref<ImageTexture> _heightMapTexture;
    TypedArray<Ref<ImageTexture>> _splatmapsTexture;
    TypedArray<Ref<ImageTexture>> _foliagesTexture;
    TypedArray<Ref<ImageTexture>> _objectsTexture;
    Ref<ImageTexture> _waterTexture;
    Ref<ImageTexture> _snowTexture;
    Ref<ImageTexture> _metaInfoTexture;

protected:
    static void _bind_methods();

public:
    ZoneResource();
    ~ZoneResource();

    Ref<ImageTexture> get_lockTexture() const;
    void set_lockTexture(const Ref<ImageTexture> &value);

    Vector2i get_zonePosition() const;
    void set_zonePosition(const Vector2i value);

    Ref<ImageTexture> get_heightMapTexture() const;
    void set_heightMapTexture(const Ref<ImageTexture> &value);

    TypedArray<Ref<ImageTexture>> get_splatmapsTexture() const;
    void set_splatmapsTexture(const TypedArray<Ref<ImageTexture>> value);

    TypedArray<Ref<ImageTexture>> get_foliagesTexture() const;
    void set_foliagesTexture(const TypedArray<Ref<ImageTexture>> value);

    TypedArray<Ref<ImageTexture>> get_objectsTexture() const;
    void set_objectsTexture(const TypedArray<Ref<ImageTexture>> value);

    Ref<ImageTexture> get_waterTexture() const;
    void set_waterTexture(const Ref<ImageTexture> &value);

    Ref<ImageTexture> get_snowTexture() const;
    void set_snowTexture(const Ref<ImageTexture> &value);

    Ref<ImageTexture> get_metaInfoTexture() const;
    void set_metaInfoTexture(const Ref<ImageTexture> &value);

    void initializeImagesForTerrain();
};
#endif