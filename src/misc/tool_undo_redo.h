#ifndef TOOL_UNDO_REDO_H
#define TOOL_UNDO_REDO_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/undo_redo.hpp>
#include <godot_cpp/classes/editor_undo_redo_manager.hpp>

using namespace godot;

class ToolUndoRedo : public RefCounted {
    GDCLASS(ToolUndoRedo, RefCounted);

private:
    UndoRedo *_undoRedo = nullptr;
    EditorUndoRedoManager *_editorUndoRedo = nullptr;

protected:
    static void _bind_methods();

public:
    ToolUndoRedo();
    ~ToolUndoRedo();

    bool canUndoRedo();

    void set_undoRedo(UndoRedo *undoRedo);
    void set_editorUndoRedo(EditorUndoRedoManager *undoRedo);

    void create_action(const String &p_name);
    void commit_action();

    void add_undo_property(Object *p_object, const StringName &p_property, const Variant &p_value);
    void add_do_property(Object *p_object, const StringName &p_property, const Variant &p_value);

    template <typename... Args>
    void add_undo_method(Object *p_object, const StringName &p_method, const Args &...p_args) {
        if (_undoRedo != nullptr) {
            _undoRedo->add_undo_method(Callable(p_object, p_method).bind(p_args...));
        } else if (_editorUndoRedo != nullptr) {
            _editorUndoRedo->add_undo_method(p_object, p_method, p_args...);
        }
    }
    template <typename... Args>
    void add_do_method(Object *p_object, const StringName &p_method, const Args &...p_args) {
        if (_undoRedo != nullptr) {
            _undoRedo->add_do_method(Callable(p_object, p_method).bind(p_args...));
        } else if (_editorUndoRedo != nullptr) {
            _editorUndoRedo->add_do_method(p_object, p_method, p_args...);
        }
    }
};
#endif