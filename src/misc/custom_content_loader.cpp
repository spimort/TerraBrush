#include "custom_content_loader.h"
#include "setting_contants.h"
#include "../editor_nodes/dock_preview_button.h"
#include "../editor_resources/object_definition_resource.h"
#include "../editor_resources/object_resource.h"
#include "../editor_resources/foliage_resource.h"
#include "../editor_resources/foliage_definition_resource.h"
#include "../editor_resources/texture_set_resource.h"
#include "../editor_resources/meta_info_layer_resource.h"

#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

using namespace godot;

void CustomContentLoader::addBrushesPreviewToParent(Node *parentNode, Callable onSelect, bool useCircleIcon) {
    TypedArray<Ref<DirAccess>> directories = TypedArray<Ref<DirAccess>>();
    directories.append(DirAccess::open("res://addons/terrabrush/Assets/Brushes/"));

    String customBrushesDirectory = ProjectSettings::get_singleton()->get_setting(SettingContants::CustomBrushesFolder());
    if (!customBrushesDirectory.is_empty() && DirAccess::dir_exists_absolute(customBrushesDirectory)) {
        directories.append(DirAccess::open(customBrushesDirectory));
    }

    int index = 0;
    for (int directoryIndex = 0; directoryIndex < directories.size(); directoryIndex++) {
        Ref<DirAccess> directory = directories[directoryIndex];

        PackedStringArray files = directory->get_files();
        for (int fileIndex = 0; fileIndex < files.size(); fileIndex++) {
            String file = files[fileIndex];

            if (file.ends_with(".png") || file.ends_with(".PNG")) {
                Ref<Texture2D> brushImage = ResourceLoader::get_singleton()->load(directory->get_current_dir() + "/" + file);
                DockPreviewButton *dockPreviewButton = memnew(DockPreviewButton);
                dockPreviewButton->set_iconType(useCircleIcon ? DockPreviewButton::IconType::Circle : DockPreviewButton::IconType::Square);
                dockPreviewButton->set_margin(5);
                parentNode->add_child(dockPreviewButton);

                dockPreviewButton->set_buttonImage(brushImage); // We keep the original image in the ButtonImage so we can use it later.

                Ref<Image> whiteBrushImage = memnew(Image);
                whiteBrushImage->copy_from(brushImage->get_image());

                for (int x = 0; x < whiteBrushImage->get_width(); x++) {
                    for (int y = 0; y < whiteBrushImage->get_height(); y++) {
                        Color pixel = whiteBrushImage->get_pixel(x, y);
                        whiteBrushImage->set_pixel(x, y, Color(1.0f - pixel.r, 1.0f - pixel.g, 1.0f - pixel.b, pixel.a));
                    }
                }

                dockPreviewButton->setTextureImage(ImageTexture::create_from_image(whiteBrushImage));

                int currentIndex = index;
                dockPreviewButton->set_onSelect(onSelect.bind(currentIndex));

                index += 1;
            }
        }
    }
}

void CustomContentLoader::addTexturesPreviewToParent(TerraBrush *terraBrush, Node *parentNode, Callable onSelect, bool useCircleIcon) {
    if (!terraBrush->get_textureSets().is_null() && terraBrush->get_textureSets()->get_textureSets().size() > 0) {
        for (int i = 0; i < terraBrush->get_textureSets()->get_textureSets().size(); i++) {
            Ref<TextureSetResource> textureSet = terraBrush->get_textureSets()->get_textureSets()[i];
            if (textureSet.is_null()) {
                break;
            }

            DockPreviewButton *dockPreviewButton = memnew(DockPreviewButton);

            dockPreviewButton->set_iconType(useCircleIcon ? DockPreviewButton::IconType::Circle : DockPreviewButton::IconType::Square);
            dockPreviewButton->set_margin(10);

            parentNode->add_child(dockPreviewButton);

            dockPreviewButton->setTextureImage(textureSet->get_albedoTexture());
            dockPreviewButton->set_tooltip_text(
                !textureSet->get_name().is_empty()
                    ? textureSet->get_name()
                    : "Texture " + String::num_int64(i + 1)
            );

            int currentIndex = i;
            dockPreviewButton->set_onSelect(onSelect.bind(currentIndex));
        }
    }
}

void CustomContentLoader::addFoliagesPreviewToParent(TerraBrush *terraBrush, Node *parentNode, Callable onSelect, bool useCircleIcon) {
    if (terraBrush->get_foliages().size() > 0) {
        for (int i = 0; i < terraBrush->get_foliages().size(); i++) {
            Ref<FoliageResource> foliage = terraBrush->get_foliages()[i];
            if (!foliage.is_null() && !foliage->get_definition().is_null()) {
                DockPreviewButton *dockPreviewButton = memnew(DockPreviewButton);

                dockPreviewButton->set_iconType(useCircleIcon ? DockPreviewButton::IconType::Circle : DockPreviewButton::IconType::Square);
                dockPreviewButton->set_margin(5);

                parentNode->add_child(dockPreviewButton);

                if (foliage->get_definition()->get_meshMaterial().is_null() && foliage->get_definition()->get_albedoTextures().size() == 0 && !foliage->get_definition()->get_mesh().is_null()) {
                    dockPreviewButton->loadResourcePreview(foliage->get_definition()->get_mesh());
                } else if (!foliage->get_definition()->get_meshMaterial().is_null()) {
                    dockPreviewButton->loadResourcePreview(foliage->get_definition()->get_meshMaterial());
                } else if (foliage->get_definition()->get_albedoTextures().size() > 0) {
                    dockPreviewButton->loadResourcePreview(foliage->get_definition()->get_albedoTextures()[0]);
                }

                dockPreviewButton->set_tooltip_text(
                    !foliage->get_definition()->get_mesh().is_null() && !foliage->get_definition()->get_mesh()->get_path().is_empty()
                        ? foliage->get_definition()->get_mesh()->get_path().get_file()
                        : "Foliage " + String::num_int64(i + 1)
                );

                int currentIndex = i;
                dockPreviewButton->set_onSelect(onSelect.bind(currentIndex));
            }
        }
    }
}

void CustomContentLoader::addObjectsPreviewToParent(TerraBrush *terraBrush, Node *parentNode, Callable onSelect, bool useCircleIcon) {
    if (terraBrush->get_objects().size() > 0) {
        for (int i = 0; i < terraBrush->get_objects().size(); i++) {
            Ref<ObjectResource> objectItem = terraBrush->get_objects()[i];

            if (objectItem.is_null() || objectItem->get_definition().is_null()) {
                break;
            }

            if (objectItem->get_definition()->get_objectScenes().size() > 0 || objectItem->get_definition()->get_lodMeshes().size() > 0) {
                DockPreviewButton *dockPreviewButton = memnew(DockPreviewButton);

                dockPreviewButton->set_iconType(useCircleIcon ? DockPreviewButton::IconType::Circle : DockPreviewButton::IconType::Square);
                dockPreviewButton->set_margin(5);

                parentNode->add_child(dockPreviewButton);

                Ref<Resource> meshResource = objectItem->get_definition()->get_objectScenes().size() > 0 ? objectItem->get_definition()->get_objectScenes()[0] : (Ref<ObjectOctreeLODMeshDefinitionResource>((Ref<ObjectOctreeLODMeshesDefinitionResource>(objectItem->get_definition()->get_lodMeshes()[0]))->get_meshes()[0]))->get_mesh();
                dockPreviewButton->loadResourcePreview(meshResource);

                TypedArray<Ref<PackedScene>> packedScenes = objectItem->get_definition()->get_objectScenes();
                String tooltipText = "Object " + String::num_int64(i + 1);
                if (packedScenes.size() > 0) {
                    PackedStringArray sceneNames = TypedArray<String>();
                    for (Ref<PackedScene> packedScene : packedScenes) {
                        if (!packedScene.is_null() && !packedScene->get_path().get_file().is_empty()) {
                            sceneNames.append(packedScene->get_path().get_file());
                        }
                    }

                    String joinedNames = String(",").join(sceneNames);
                    if (!joinedNames.is_empty()) {
                        tooltipText = joinedNames;
                    }
                }

                dockPreviewButton->set_tooltip_text(tooltipText);

                int currentIndex = i;
                dockPreviewButton->set_onSelect(onSelect.bind(currentIndex));
            }
        }
    }
}

void CustomContentLoader::addMetaInfoLayersPreviewToParent(TerraBrush *terraBrush, Node *parentNode, Callable onSelect, bool useCircleIcon) {
    if (terraBrush->get_metaInfoLayers().size() > 0) {
        for (int i = 0; i < terraBrush->get_metaInfoLayers().size(); i++) {
            Ref<MetaInfoLayerResource> metaInfoLayer = terraBrush->get_metaInfoLayers()[i];
            if (metaInfoLayer.is_null()) {
                break;
            }

            DockPreviewButton *dockPreviewButton = memnew(DockPreviewButton);

            dockPreviewButton->set_iconType(useCircleIcon ? DockPreviewButton::IconType::Circle : DockPreviewButton::IconType::Square);
            dockPreviewButton->set_margin(10);

            parentNode->add_child(dockPreviewButton);

            Ref<Image> image = Image::create_empty(48, 48, false, Image::Format::FORMAT_RGBA8);
            image->fill(metaInfoLayer->get_color());
            Ref<ImageTexture> imageTexture = ImageTexture::create_from_image(image);
            dockPreviewButton->setTextureImage(imageTexture);
            dockPreviewButton->set_tooltip_text(
                !metaInfoLayer->get_name()
                    ? metaInfoLayer->get_name()
                    : "MetaInfo " + String::num_int64(i + 1)
            );

            int currentIndex = i;
            dockPreviewButton->set_onSelect(onSelect.bind(currentIndex));
        }
    }
}