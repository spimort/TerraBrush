#ifndef IMPORT_DIALOG_INFO_H
#define IMPORT_DIALOG_INFO_H

#include "../terra_brush.h"
#include "../misc/importer_engine.h"
#include "import_image_row.h"

#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/check_box.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/sub_viewport_container.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>

#include <functional>

using namespace godot;

class ImportDialog : public Window {
    GDCLASS(ImportDialog, Window);

private:
    TerraBrush *_previewTerrain = nullptr;
    TerraBrush *_originialTerraBrush = nullptr;
    ImportImageRow *_heightmapRow = nullptr;
    SpinBox *_heightmapScaleSpinBox = nullptr;
    CheckBox *_useGreenChannelForHolesCheckbox = nullptr;
    CheckBox *_scaleToResolutionCheckbox = nullptr;
    ImportImageRow *_colorRow = nullptr;
    VBoxContainer *_splatmapsContainer = nullptr;
    VBoxContainer *_foliagesContainer = nullptr;
    VBoxContainer *_objectsContainer = nullptr;
    ImportImageRow *_waterRow = nullptr;
    ImportImageRow *_snowRow = nullptr;
    ImportImageRow *_metaInfoRow = nullptr;
    SubViewportContainer *_subViewportContainer = nullptr;
    SubViewport *_subViewport = nullptr;

    std::function<void(ImporterSettings)> _onAccept = nullptr;

    void buildLayout();
    void previewImport();
    ImporterSettings getImporterSettings();
    void onOkButton();
    void onCancelButton();

protected:
    static void _bind_methods();

public:
    ImportDialog();
    ~ImportDialog();

    void init(std::function<void(ImporterSettings)> onAccept);

    void _ready() override;

    void set_originialTerraBrush(TerraBrush *value);
};
#endif