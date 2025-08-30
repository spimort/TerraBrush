#include "terra_brush_plugin.h"

using namespace godot;

void TerraBrushPlugin::_bind_methods() {}

TerraBrushPlugin::TerraBrushPlugin() {}

TerraBrushPlugin::~TerraBrushPlugin() {}

void TerraBrushPlugin::_physics_process(double delta) {
    if (!_isMousePressed) {
        _updateTime = 0;
    } else if (_updateTime > 0) {
        _updateTime -= (float) delta;
    } else if (_isMousePressed && _mouseHitPosition.is_finite() && !_currentTool.is_null()) {
        int zoneSize = _currentTerraBrushNode->get_zonesSize();
        Vector3 meshToImagePosition = _mouseHitPosition + Vector3(zoneSize / 2, 0, zoneSize / 2);
        Vector2 imagePosition = Vector2(meshToImagePosition.x, meshToImagePosition.z);

        _currentTool->paint(_currentToolType, _brushImage, _brushSize, _brushStrength, imagePosition);

        _updateTime = UpdateDelay;
    }
}

void TerraBrushPlugin::_enter_tree() {

}

void TerraBrushPlugin::_exit_tree() {

}

void TerraBrushPlugin::_edit(Object *object) {

}

bool TerraBrushPlugin::_handles(Object *object) const {

}

void TerraBrushPlugin::_save_external_data() {

}

int TerraBrushPlugin::_forward_3d_gui_input(Camera3D *viewportCamera, const Ref<InputEvent> &event) {

}

void TerraBrushPlugin::createCustomSetting(String name, Variant defaultValue, GDExtensionVariantType type, PropertyHint hint, String hintString) {

}

void TerraBrushPlugin::handleKeyBindings() {

}

void TerraBrushPlugin::onUndoTexture(Ref<ImageTexture> imageTexture, PackedByteArray previousImageData) {

}

void TerraBrushPlugin::onUndoRedo() {

}

Vector3 TerraBrushPlugin::getRayCastWithTerrain(Camera3D *editorCamera) {

}

Vector3 TerraBrushPlugin::getMouseClickToZoneHeight(Vector3 from, Vector3 direction) {

}

void TerraBrushPlugin::removeDock() {

}

void TerraBrushPlugin::onEditTerrainNode(TerraBrush *terraBrush) {

}

void TerraBrushPlugin::addDock() {

}

void TerraBrushPlugin::onExitEditTerrainNode() {

}

Node TerraBrushPlugin::*getEditorViewportsContainer() {

}

Node TerraBrushPlugin::*getEditorViewportsContainerRecursive(Node *node) {

}

Node TerraBrushPlugin::*getActiveViewport() {

}

String TerraBrushPlugin::hideOverlaySelector() {

}

void TerraBrushPlugin::showToolPieMenu(StringName actionName) {

}

void TerraBrushPlugin::showCustomContentPieMenu(String label, Callable addItems) {

}

void TerraBrushPlugin::showCurrentToolMenu() {

}

void TerraBrushPlugin::showBrushNumericSelector(int minVale, int maxValue, Color widgetColor, int initialValue, Callable onValueSelected, StringName actionName) {

}

void TerraBrushPlugin::updateTerrainSettings() {

}

void TerraBrushPlugin::updateAutoAddZonesSetting() {

}
