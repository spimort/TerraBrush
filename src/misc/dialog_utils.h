#ifndef DIALOG_UTILS_H
#define DIALOG_UTILS_H

#include "../terra_brush.h"
#include "../misc/importer_engine.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/editor_file_dialog.hpp>

#include <functional>

using namespace godot;

class DialogUtils {
    public:
        static void showFileDialog(Node *sourceNode, std::function<void(String)> onSelect, EditorFileDialog::Access access = EditorFileDialog::Access::ACCESS_FILESYSTEM, EditorFileDialog::FileMode fileMode = EditorFileDialog::FileMode::FILE_MODE_OPEN_FILE, TypedArray<String> filters = TypedArray<String>(), bool transient = false);
        static void showNumericSelector(Node *sourceNode, std::function<void(float)> onSelect, float defaultValue = 0, float minValue = std::numeric_limits<float>::min(), float maxValue = std::numeric_limits<float>::max());
        static void showConfirmDialog(Node *sourceNode, String title, String content, std::function<void(bool)> onSelect);
        static void showImportDialog(Node *sourceNode, TerraBrush *originalTerraBursh, std::function<void(ImporterSettings)> onAccept);
};

class FileDialogEventsWrapper : public Node {
    GDCLASS(FileDialogEventsWrapper, Node);

private:
    std::function<void(String)> _onSelect = nullptr;
    std::function<void()> _onCancel = nullptr;

    void onSelect(String value);
    void onCancel();

protected:
    static void _bind_methods();

public:
    FileDialogEventsWrapper();
    ~FileDialogEventsWrapper();

    void init(std::function<void(String)> onSelectCallback, std::function<void()> onCancelCallback);
};
#endif