#include "zones_resource.h"

using namespace godot;

void ZonesResource::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_zones"), &ZonesResource::get_zones);
    ClassDB::bind_method(D_METHOD("set_zones", "value"), &ZonesResource::set_zones);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "zones", PROPERTY_HINT_TYPE_STRING, "24/17:ZoneResource"), "set_zones", "get_zones");
}

ZonesResource::ZonesResource() {
    _dirtyImageTextures = HashSet<ImageTexture>();

    _lockTextures = memnew(Ref<Texture2DArray>);
    _heightmapTextures = memnew(Ref<Texture2DArray>);
    _splatmapsTextures = memnew(Ref<Texture2DArray>);
    _objectsTextures = memnew(Ref<Texture2DArray>);
    _waterTextures = memnew(Ref<Texture2DArray>);
    _snowTextures = memnew(Ref<Texture2DArray>);
    _metaInfoTextures = memnew(Ref<Texture2DArray>);
    _zonesMap = memnew(Ref<ImageTexture>);
}

ZonesResource::~ZonesResource() {}

Ref<Texture2DArray> ZonesResource::get_lockTextures() const {
    return _lockTextures;
}
Ref<Texture2DArray> ZonesResource::get_heightmapTextures() const {
    return _heightmapTextures;
}
Ref<Texture2DArray> ZonesResource::get_splatmapsTextures() const {
    return _splatmapsTextures;
}
TypedArray<Ref<Texture2DArray>> ZonesResource::get_foliagesTextures() const {
    return _foliagesTextures;
}
Ref<Texture2DArray> ZonesResource::get_objectsTextures() const {
    return _objectsTextures;
}
Ref<Texture2DArray> ZonesResource::get_waterTextures() const {
    return _waterTextures;
}
Ref<Texture2DArray> ZonesResource::get_snowTextures() const {
    return _snowTextures;
}
Ref<Texture2DArray> ZonesResource::get_metaInfoTextures() const {
    return _metaInfoTextures;
}
Ref<ImageTexture> ZonesResource::get_zonesMap() const {
    return _zonesMap;
}

TypedArray<Ref<ZoneResource>> ZonesResource::get_zones() const {
    return _zones;
}
void ZonesResource::set_zones(const TypedArray<Ref<ZoneResource>> value) {
    _zones = value;
}

// TODO : GDExtension
// void ZonesResource::updateLockTexture(int zoneSize) {
//     var images = Zones.Select(zone => zone.LockTexture?.GetImage() ?? GodotAgnostic.ImageCreateEmpty(zoneSize, zoneSize, false, Image.Format.Rf)).ToArray();

//     if (images.Length != 0) {
//         _lockTextures.CreateFromImages(new GodotArray<Image>([..images]));
//     }
// }

// void ZonesResource::updateHeightmaps() {
//     var images = Zones.Select(zone => zone.HeightMapTexture.GetImage()).ToArray();
//     if (images.Length != 0) {
//         _heightmapTextures.CreateFromImages(new GodotArray<Image>([..images]));
//     }
// }

// void ZonesResource::updateSplatmapsTextures() {
//     var images = Zones.Aggregate(new List<Image>(), (source, zone) => {
//         if (zone.SplatmapsTexture != null) {
//             source.AddRange(zone.SplatmapsTexture.Select(texture => texture.GetImage()));
//         }

//         return source;
//     });

//     if (images.Count > 0) {
//         _splatmapsTextures.CreateFromImages(new GodotArray<Image>([..images]));
//     }
// }

// void ZonesResource::initializeFoliageTextures(TerraBrush terraBrush) {
//     _foliagesTextures = terraBrush.Foliages?.Select(_ => new Texture2DArray()).ToArray();
// }

// void ZonesResource::updateFoliagesTextures() {
//     if (_foliagesTextures?.Length <= 0) return;

//     for (var i = 0; i < _foliagesTextures?.Length; i++) {
//         UpdateFoliagesTextures(i);
//     }
// }

// void ZonesResource::updateFoliagesTextures(int foliageIndex) {
//     var images = Zones.Select(zone => zone.FoliagesTexture[foliageIndex].GetImage()).ToArray();

//     if (images.Length > 0) {
//         _foliagesTextures[foliageIndex].CreateFromImages(new GodotArray<Image>([..images]));
//     }
// }

// void ZonesResource::updateObjectsTextures() {
//     var images = Zones.Aggregate(new List<Image>(), (source, zone) => {
//         if (zone.ObjectsTexture != null) {
//             source.AddRange(zone.ObjectsTexture.Select(texture => texture.GetImage()));
//         }

//         return source;
//     });

//     if (images.Count > 0) {
//         _objectsTextures.CreateFromImages(new GodotArray<Image>([..images]));
//     }
// }

// void ZonesResource::updateWaterTextures() {
//     if (Zones.Any(zone => zone.WaterTexture == null)) {
//         return;
//     }

//     _waterTextures.CreateFromImages(new GodotArray<Image>([..Zones.Select(zone => zone.WaterTexture.GetImage())]));
// }

// void ZonesResource::updateZoneWaterTexture(ZoneResource zone) {
//     _waterTextures.UpdateLayer(zone.WaterTexture.GetImage(), Array.IndexOf([..Zones], zone));
// }

// void ZonesResource::updateSnowTextures() {
//     if (Zones.Any(zone => zone.SnowTexture == null)) {
//         return;
//     }

//     _snowTextures.CreateFromImages(new GodotArray<Image>([..Zones.Select(zone => zone.SnowTexture.GetImage())]));
// }

// void ZonesResource::updateZoneSnowTexture(ZoneResource zone) {
//     _snowTextures.UpdateLayer(zone.SnowTexture.GetImage(), Array.IndexOf([..Zones], zone));
// }

// void ZonesResource::updateMetaInfoTextures() {
//     if (Zones.Any(zone => zone.MetaInfoTexture == null)) {
//         return;
//     }

//     _metaInfoTextures.CreateFromImages(new GodotArray<Image>([..Zones.Select(zone => zone.MetaInfoTexture.GetImage())]));
// }

// void ZonesResource::updateZoneMetaInfoTexture(ZoneResource zone) {
//     _metaInfoTextures.UpdateLayer(zone.MetaInfoTexture.GetImage(), Array.IndexOf([..Zones], zone));
// }

// void ZonesResource::saveResources() {
//     foreach (var dirtyImageResource in _dirtyImageTextures) {
//         SaveImageResource(dirtyImageResource);
//     }

//     _dirtyImageTextures.Clear();
// }

// void ZonesResource::saveImageResource(ImageTexture image) {
//     if (!string.IsNullOrWhiteSpace(image.ResourcePath) && Godot.FileAccess.FileExists(image.ResourcePath)) {
//         ResourceSaver.Singleton.Save(image, image.ResourcePath);
//     }
// }

// void ZonesResource::updateZonesMap() {
//     var zonePositions = Zones.Select(zone => zone.ZonePosition).ToArray();
//     var maxX = zonePositions.Max(x => Math.Abs(x.X));
//     var maxY = zonePositions.Max(x => Math.Abs(x.Y));

//     var zonesMap = GodotAgnostic.ImageCreateEmpty((maxX * 2) + 1, (maxY * 2) + 1, false, Image.Format.Rf);
//     zonesMap.Fill(new Color(-1, 0, 0, 0));
//     for (var i = 0; i < zonePositions.Length; i++) {
//         var position = zonePositions[i];
//         zonesMap.SetPixel(position.X + maxX, position.Y + maxY, new Color(i, 0, 0, 0));
//     }

//     _zonesMap.SetImage(zonesMap);
// }

// ZoneResource ZonesResource::addNewZone(TerraBrush terraBrush, Vector2I zonePosition) {
//     var zone = new ZoneResource() {
//         ZonePosition = zonePosition
//     };

//     zone.InitializeImagesForTerrain(terraBrush);

//     var newList = new List<ZoneResource>(Zones) {
//         zone
//     };
//     Zones = new GodotArray<ZoneResource>([..newList]);

//     UpdateImageTextures(terraBrush.ZonesSize);

//     terraBrush.Terrain?.Clipmap?.UpdateAABB();
//     terraBrush.Water?.Clipmap?.UpdateAABB();
//     terraBrush.Snow?.Clipmap?.UpdateAABB();

//     return zone;
// }

// void ZonesResource::addDirtyImageTexture(ImageTexture imageTexture) {
//     _dirtyImageTextures.Add(imageTexture);
// }

// void ZonesResource::updateImageTextures(int zoneSize) {
//     if (Engine.Singleton.IsEditorHint()) {
//         UpdateLockTexture(zoneSize);
//     }
//     UpdateHeightmaps();
//     UpdateSplatmapsTextures();
//     UpdateFoliagesTextures();
//     UpdateObjectsTextures();
//     UpdateWaterTextures();
//     UpdateSnowTextures();
//     UpdateMetaInfoTextures();
//     UpdateZonesMap();
// }

// ZoneResource ZonesResource::getZoneForZoneInfo(ZoneInfo zoneInfo) {
//     return Zones?.FirstOrDefault(x => x.ZonePosition.X == zoneInfo.ZonePosition.X && x.ZonePosition.Y == zoneInfo.ZonePosition.Y);
// }