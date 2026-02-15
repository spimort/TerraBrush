#include "tool_undo_redo.h"

using namespace godot;

void ToolUndoRedo::_bind_methods() {}

ToolUndoRedo::ToolUndoRedo() {}

ToolUndoRedo::~ToolUndoRedo() {}

bool ToolUndoRedo::canUndoRedo() {
    return _undoRedo != nullptr || _editorUndoRedo != nullptr;
}

void ToolUndoRedo::set_undoRedo(UndoRedo *undoRedo) {
    _undoRedo = undoRedo;
}

void ToolUndoRedo::set_editorUndoRedo(EditorUndoRedoManager *undoRedo) {
    _editorUndoRedo = undoRedo;
}

void ToolUndoRedo::create_action(const String &p_name) {
    if (_undoRedo != nullptr) {
        _undoRedo->create_action(p_name);
    } else if (_editorUndoRedo != nullptr) {
        _editorUndoRedo->create_action(p_name);
    }
}

void ToolUndoRedo::commit_action() {
    if (_undoRedo != nullptr) {
        _undoRedo->commit_action();
    } else if (_editorUndoRedo != nullptr) {
        _editorUndoRedo->commit_action();
    }
}

void ToolUndoRedo::add_undo_property(Object *p_object, const StringName &p_property, const Variant &p_value) {
    if (_undoRedo != nullptr) {
        _undoRedo->add_undo_property(p_object, p_property, p_value);
    } else if (_editorUndoRedo != nullptr) {
        _editorUndoRedo->add_undo_property(p_object, p_property, p_value);
    }
}

void ToolUndoRedo::add_do_property(Object *p_object, const StringName &p_property, const Variant &p_value) {
    if (_undoRedo != nullptr) {
        _undoRedo->add_do_property(p_object, p_property, p_value);
    } else if (_editorUndoRedo != nullptr) {
        _editorUndoRedo->add_do_property(p_object, p_property, p_value);
    }
}