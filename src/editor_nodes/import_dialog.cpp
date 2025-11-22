#include "import_dialog.h"
#include "import_image_row.h"

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/label_settings.hpp>
#include <godot_cpp/classes/style_box_flat.hpp>
#include <godot_cpp/classes/sub_viewport_container.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/environment.hpp>
#include <godot_cpp/classes/sky.hpp>
#include <godot_cpp/classes/procedural_sky_material.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/directional_light3d.hpp>

using namespace godot;

void ImportDialog::_bind_methods() {
    ClassDB::bind_method(D_METHOD("previewImport"), &ImportDialog::previewImport);
    ClassDB::bind_method(D_METHOD("onOkButton"), &ImportDialog::onOkButton);
    ClassDB::bind_method(D_METHOD("onCancelButton"), &ImportDialog::onCancelButton);
}

ImportDialog::ImportDialog() {}

ImportDialog::~ImportDialog() {}

void ImportDialog::init(std::function<void(ImporterSettings)> onAccept) {
    _onAccept = onAccept;
}

void ImportDialog::_ready() {
    set_title("Import Terrain");
    set_initial_position(WindowInitialPosition::WINDOW_INITIAL_POSITION_CENTER_MAIN_WINDOW_SCREEN);
    set_size(Vector2(800, 600));
    set_exclusive(true);
    connect("close_requested", Callable(this, "onCancelButton"));

    buildLayout();
}

void ImportDialog::set_originialTerraBrush(TerraBrush *value) {
    _originialTerraBrush = value;
}

void ImportDialog::previewImport() {
    ImporterSettings settings = getImporterSettings();
    if (settings.heightmap.is_null()) {
        return;
    }

    _subViewportContainer->set_visible(true);

    if (_previewTerrain != nullptr) {
        _previewTerrain->queue_free();
    }
    _previewTerrain = memnew(TerraBrush);
    _previewTerrain->set_zonesSize(_originialTerraBrush->get_zonesSize());
    _previewTerrain->set_resolution(_originialTerraBrush->get_resolution());
    ImporterEngine::importTerrain(_previewTerrain, settings);
    _subViewport->add_child(_previewTerrain);
}

void ImportDialog::onOkButton() {
    ImporterSettings settings = getImporterSettings();
    _onAccept(settings);
    queue_free();
}

void ImportDialog::onCancelButton() {
    queue_free();
}

ImporterSettings ImportDialog::getImporterSettings() {
    ImporterSettings settings = ImporterSettings();

    settings.heightmap = _heightmapRow->get_imageTexture();

    if (_heightmapScaleSpinBox != nullptr) {
        settings.heightmapScale = _heightmapScaleSpinBox->get_value();
    }

    if (_useGreenChannelForHolesCheckbox != nullptr) {
        settings.useGreenChannelForHoles = _useGreenChannelForHolesCheckbox->is_pressed();
    }

    if (_scaleToResolutionCheckbox != nullptr) {
        settings.scaleToResolution = _scaleToResolutionCheckbox->is_pressed();
    }

    if (_splatmapsContainer != nullptr && _splatmapsContainer->get_child_count() > 0) {
        TypedArray<Ref<Texture2D>> splatmaps = TypedArray<Ref<Texture2D>>();

        for (int i = 0; i < _splatmapsContainer->get_child_count(); i++) {
            ImportImageRow *importRow = Object::cast_to<ImportImageRow>(_splatmapsContainer->get_child(i));
            Ref<Texture2D> texture = importRow->get_imageTexture();
            if (!texture.is_null()) {
                splatmaps.append(texture);
            }
        }

        settings.splatmaps = splatmaps;
    }

    if (_foliagesContainer != nullptr) {
        TypedArray<Ref<Texture2D>> foliages = TypedArray<Ref<Texture2D>>();

        for (int i = 0; i < _foliagesContainer->get_child_count(); i++) {
            ImportImageRow *importRow = Object::cast_to<ImportImageRow>(_foliagesContainer->get_child(i));
            Ref<Texture2D> texture = importRow->get_imageTexture();
            if (!texture.is_null()) {
                foliages.append(texture);
            }
        }

        settings.foliages = foliages;
    }

    if (_objectsContainer != nullptr) {
        TypedArray<Ref<Texture2D>> objects = TypedArray<Ref<Texture2D>>();

        for (int i = 0; i < _objectsContainer->get_child_count(); i++) {
            ImportImageRow *importRow = Object::cast_to<ImportImageRow>(_objectsContainer->get_child(i));
            Ref<Texture2D> texture = importRow->get_imageTexture();
            if (!texture.is_null()) {
                objects.append(texture);
            }
        }

        settings.objects = objects;
    }

    if (_waterRow != nullptr) {
        settings.water = _waterRow->get_imageTexture();
    }

    if (_snowRow != nullptr) {
        settings.snow = _snowRow->get_imageTexture();
    }

    if (_metaInfoRow != nullptr) {
        settings.metaInfo = _metaInfoRow->get_imageTexture();
    }

    return settings;
}

void ImportDialog::buildLayout() {
    if (_originialTerraBrush == nullptr) {
        return;
    }

    HBoxContainer *hboxContainer = memnew(HBoxContainer);
    hboxContainer->set_anchors_and_offsets_preset(Control::LayoutPreset::PRESET_FULL_RECT);
    hboxContainer->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
    hboxContainer->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
    add_child(hboxContainer);

    { // HBoxContainer
        VBoxContainer *vboxContainer = memnew(VBoxContainer);
        vboxContainer->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
        hboxContainer->add_child(vboxContainer);

        { // VBoxContainer
            MarginContainer *marginContainer = memnew(MarginContainer);
            marginContainer->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
            marginContainer->set("theme_override_constants/margin_left", 10);
            marginContainer->set("theme_override_constants/margin_top", 10);
            marginContainer->set("theme_override_constants/margin_right", 10);
            marginContainer->set("theme_override_constants/margin_bottom", 10);
            vboxContainer->add_child(marginContainer);

            { // MarginContainer
                VBoxContainer *contentVboxContainer = memnew(VBoxContainer);
                contentVboxContainer->set("theme_override_constants/separation", 10);
                marginContainer->add_child(contentVboxContainer);

                { // VBoxContainer
                    ScrollContainer *scrollContainer = memnew(ScrollContainer);
                    scrollContainer->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
                    contentVboxContainer->add_child(scrollContainer);

                    { // ScrollContainer
                        VBoxContainer *rowsVboxContainer = memnew(VBoxContainer);
                        rowsVboxContainer->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
                        rowsVboxContainer->set("theme_override_constants/separation", 10);
                        scrollContainer->add_child(rowsVboxContainer);

                        { // VBoxContainer
                            _heightmapRow = memnew(ImportImageRow);
                            _heightmapRow->set_imageTypeName("Heightmap");
                            rowsVboxContainer->add_child(_heightmapRow);

                            if (!_originialTerraBrush->get_textureSets().is_null() && _originialTerraBrush->get_textureSets()->get_textureSets().size() > 0) {
                                _splatmapsContainer = memnew(VBoxContainer);
                                _splatmapsContainer->set("theme_override_constants/separation", 10);
                                rowsVboxContainer->add_child(_splatmapsContainer);

                                int numberOfSplatmaps = (int) Math::ceil(_originialTerraBrush->get_textureSets()->get_textureSets().size() / 4.0f);
                                for (int i = 0; i < numberOfSplatmaps; i++) {
                                    ImportImageRow *importRow = memnew(ImportImageRow);
                                    importRow->set_imageTypeName("Splatmap " + String::num_int64(i + 1));
                                    _splatmapsContainer->add_child(importRow);
                                }
                            }

                            if (_originialTerraBrush->get_foliages().size() > 0) {
                                _foliagesContainer = memnew(VBoxContainer);
                                _foliagesContainer->set("theme_override_constants/separation", 10);
                                rowsVboxContainer->add_child(_foliagesContainer);

                                for (int i = 0; i < _originialTerraBrush->get_foliages().size(); i++) {
                                    ImportImageRow *importRow = memnew(ImportImageRow);
                                    importRow->set_imageTypeName("Foliage " + String::num_int64(i + 1));
                                    _foliagesContainer->add_child(importRow);
                                }
                            }

                            if (_originialTerraBrush->get_objects().size() > 0) {
                                _objectsContainer = memnew(VBoxContainer);
                                _objectsContainer->set("theme_override_constants/separation", 10);
                                rowsVboxContainer->add_child(_objectsContainer);

                                for (int i = 0; i < _originialTerraBrush->get_objects().size(); i++) {
                                    ImportImageRow *importRow = memnew(ImportImageRow);
                                    importRow->set_imageTypeName("Object " + String::num_int64(i + 1));
                                    _objectsContainer->add_child(importRow);
                                }
                            }

                            if (!_originialTerraBrush->get_waterDefinition().is_null()) {
                                _waterRow = memnew(ImportImageRow);
                                _waterRow->set_imageTypeName("Water");
                                rowsVboxContainer->add_child(_waterRow);
                            }

                            if (!_originialTerraBrush->get_snowDefinition().is_null()) {
                                _snowRow = memnew(ImportImageRow);
                                _snowRow->set_imageTypeName("Snow");
                                rowsVboxContainer->add_child(_snowRow);
                            }

                            if (_originialTerraBrush->get_metaInfoLayers().size() > 0) {
                                _metaInfoRow = memnew(ImportImageRow);
                                _metaInfoRow->set_imageTypeName("MetaInfo");
                                rowsVboxContainer->add_child(_metaInfoRow);
                            }
                        }
                    }

                    HBoxContainer *useGreenChannelContainer = memnew(HBoxContainer);
                    contentVboxContainer->add_child(useGreenChannelContainer);

                    { // HBoxContainer2
                        _useGreenChannelForHolesCheckbox = memnew(CheckBox);
                        _useGreenChannelForHolesCheckbox->set_text("Use green channel for holes");
                        useGreenChannelContainer->add_child(_useGreenChannelForHolesCheckbox);
                    }

                    HBoxContainer *scaleToResolutionContainer = memnew(HBoxContainer);
                    contentVboxContainer->add_child(scaleToResolutionContainer);

                    { // HBoxContainer3
                        _scaleToResolutionCheckbox = memnew(CheckBox);
                        _scaleToResolutionCheckbox->set_text("Scale to match resolution");
                        _scaleToResolutionCheckbox->set_pressed(true);
                        scaleToResolutionContainer->add_child(_scaleToResolutionCheckbox);
                    }

                    HBoxContainer *heightmapScaleSpinContainer = memnew(HBoxContainer);
                    contentVboxContainer->add_child(heightmapScaleSpinContainer);

                    { // HBoxContainer4
                        Label *label = memnew(Label);
                        label->set_text("Heightmap Scale");
                        label->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
                        heightmapScaleSpinContainer->add_child(label);

                        _heightmapScaleSpinBox = memnew(SpinBox);
                        _heightmapScaleSpinBox->set_value(1.0);
                        _heightmapScaleSpinBox->set_min(-100000);
                        _heightmapScaleSpinBox->set_max(100000);
                        _heightmapScaleSpinBox->set_step(0.01);
                        heightmapScaleSpinContainer->add_child(_heightmapScaleSpinBox);
                    }

                    Ref<LabelSettings> labelSettings = memnew(LabelSettings);
                    labelSettings->set_font_color(Color::html("#ffc000"));

                    Label *infoLabel = memnew(Label);
                    infoLabel->set_text("Adjust the heightmap scale (default is 1). A scale of 100 will set the highest point of the terrain to 100 if the heightmap's highest point is 1. Use an EXR file for more precision.");
                    infoLabel->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
                    infoLabel->set_autowrap_mode(TextServer::AutowrapMode::AUTOWRAP_WORD);
                    infoLabel->set_label_settings(labelSettings);
                    contentVboxContainer->add_child(infoLabel);
                }
            }

            MarginContainer *buttonsMarginContainer = memnew(MarginContainer);
            buttonsMarginContainer->set("theme_override_constants/margin_left", 10);
            buttonsMarginContainer->set("theme_override_constants/margin_top", 10);
            buttonsMarginContainer->set("theme_override_constants/margin_right", 10);
            buttonsMarginContainer->set("theme_override_constants/margin_bottom", 10);
            vboxContainer->add_child(buttonsMarginContainer);

            { // MarginContainer2
                HBoxContainer *buttonsContainer = memnew(HBoxContainer);
                buttonsContainer->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
                buttonsMarginContainer->add_child(buttonsContainer);

                { // HBoxContainer
                    Button *previewButton = memnew(Button);
                    previewButton->set_text("   Preview   ");
                    previewButton->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
                    previewButton->connect("pressed", Callable(this, "previewImport"));
                    buttonsContainer->add_child(previewButton);

                    Button *okButton = memnew(Button);
                    okButton->set_text("   OK   ");
                    okButton->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
                    okButton->connect("pressed", Callable(this, "onOkButton"));
                    buttonsContainer->add_child(okButton);

                    Button *cancelButton = memnew(Button);
                    cancelButton->set_text("   Cancel   ");
                    cancelButton->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
                    cancelButton->connect("pressed", Callable(this, "onCancelButton"));
                    buttonsContainer->add_child(cancelButton);
                }
            }
        }

        Ref<StyleBoxFlat> previewPanelContainerStyle = memnew(StyleBoxFlat);
        previewPanelContainerStyle->set_bg_color(Color::html("#1a1a1a"));

        PanelContainer *previewPanelContainer = memnew(PanelContainer);
        previewPanelContainer->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
        previewPanelContainer->set("theme_override_styles/panel", previewPanelContainerStyle);
        hboxContainer->add_child(previewPanelContainer);

        { // PanelContainer
            Label *previewLabel = memnew(Label);
            previewLabel->set_text("Preview");
            previewLabel->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
            previewPanelContainer->add_child(previewLabel);

            _subViewportContainer = memnew(SubViewportContainer);
            _subViewportContainer->set_stretch(true);
            _subViewportContainer->set_visible(false);
            previewPanelContainer->add_child(_subViewportContainer);

            { // SubViewportContainer
                Ref<ProceduralSkyMaterial> proceduralSky = memnew(ProceduralSkyMaterial);
                proceduralSky->set_sky_horizon_color(Color::html("#a5a7ab"));
                proceduralSky->set_ground_horizon_color(Color::html("#a5a7ab"));

                Ref<Sky> sky = memnew(Sky);
                sky->set_material(proceduralSky);

                Ref<Environment> environment = memnew(Environment);
                environment->set_background(Environment::BGMode::BG_SKY);
                environment->set_sky(sky);
                environment->set_tonemapper(Environment::ToneMapper::TONE_MAPPER_FILMIC);
                environment->set_glow_enabled(true);

                Ref<World3D> ownWorld = memnew(World3D);
                ownWorld->set_environment(environment);

                _subViewport = memnew(SubViewport);
                _subViewport->set_use_own_world_3d(true);
                _subViewport->set_world_3d(ownWorld);
                _subViewport->set_handle_input_locally(false);
                _subViewport->set_disable_input(true);
                _subViewport->set_update_mode(SubViewport::UpdateMode::UPDATE_DISABLED);
                _subViewportContainer->add_child(_subViewport);

                { // Subviewport
                    Camera3D *camera = memnew(Camera3D);
                    camera->set_current(true);
                    camera->set_position(Vector3(-204.624, 630.456, -196.121));
                    camera->set_rotation_degrees(Vector3(-39.4, -131.4, 0));
                    _subViewport->add_child(camera);

                    DirectionalLight3D *directionalLight = memnew(DirectionalLight3D);
                    directionalLight->set_shadow(true);
                    directionalLight->set_rotation_degrees(Vector3(-60.0, 150.0, 0));
                    _subViewport->add_child(directionalLight);
                }
            }
        }
    }
}