#include "dialog_utils.h"
#include "../editor_nodes/numeric_selector_dialog.h"
#include "../editor_nodes/import_dialog.h"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/confirmation_dialog.hpp>

using namespace godot;

void DialogUtils::showFileDialog(Node *sourceNode, std::function<void(String)> onSelect, EditorFileDialog::Access access, EditorFileDialog::FileMode fileMode, TypedArray<String> filters, bool transient) {
    EditorFileDialog *fileDialog = memnew(EditorFileDialog);
    fileDialog->set_access(access);
    fileDialog->set_file_mode(fileMode);
    fileDialog->set_filters(filters);
    fileDialog->set_exclusive(true);
    fileDialog->set_transient(transient);

    FileDialogEventsWrapper *eventsWrapper = memnew(FileDialogEventsWrapper);
    eventsWrapper->init(
        ([fileDialog, onSelect](String value) {
            fileDialog->queue_free();
            onSelect(value);
        }),
        ([fileDialog, onSelect] {
            fileDialog->queue_free();
            onSelect("");
        })
    );

    fileDialog->connect("file_selected", Callable(eventsWrapper, "onSelect"));
    fileDialog->connect("dir_selected", Callable(eventsWrapper, "onSelect"));

    fileDialog->connect("canceled", Callable(eventsWrapper, "onCancel"));
    fileDialog->connect("close_requested", Callable(eventsWrapper, "onCancel"));

    sourceNode->add_child(fileDialog);
    fileDialog->add_child(eventsWrapper);
    fileDialog->popup_centered(Vector2i(800, 600));
}

void DialogUtils::showNumericSelector(Node *sourceNode, std::function<void(float)> onSelect, float defaultValue, float minValue, float maxValue) {
    NumericSelectorDialog *dialog = memnew(NumericSelectorDialog);
    dialog->init(
        minValue,
        maxValue,
        onSelect
    );

    sourceNode->get_tree()->get_root()->add_child(dialog);
    dialog->popup_centered();

    dialog->setValue(defaultValue);
}

void DialogUtils::showConfirmDialog(Node *sourceNode, String title, String content, std::function<void(bool)> onConfirm) {
    ConfirmationDialog *dialog = memnew(ConfirmationDialog);
    dialog->set_title(title);
    dialog->set_text(content);

    ConfirmationDialogEventsWrapper *eventsWrapper = memnew(ConfirmationDialogEventsWrapper);
    eventsWrapper->init(
        ([dialog, onConfirm](bool value) {
            dialog->queue_free();
            onConfirm(value);
        })
    );

    dialog->connect("canceled", Callable(eventsWrapper, "onConfirm").bind(false));
    dialog->connect("confirmed", Callable(eventsWrapper, "onConfirm").bind(true));

    sourceNode->add_child(dialog);
    dialog->add_child(eventsWrapper);
    dialog->popup_centered();
}

void DialogUtils::showImportDialog(Node *sourceNode, TerraBrush *originalTerraBrush, std::function<void(ImporterSettings)> onAccept) {
    ImportDialog *importDialog = memnew(ImportDialog);
    importDialog->init(onAccept);
    importDialog->set_originialTerraBrush(originalTerraBrush);

    sourceNode->get_tree()->get_root()->add_child(importDialog);
    importDialog->popup_centered();
}

// FileDialogEventsWrapper

void FileDialogEventsWrapper::_bind_methods() {
    ClassDB::bind_method(D_METHOD("onSelect", "value"), &FileDialogEventsWrapper::onSelect);
    ClassDB::bind_method(D_METHOD("onCancel"), &FileDialogEventsWrapper::onCancel);
}

FileDialogEventsWrapper::FileDialogEventsWrapper() {}

FileDialogEventsWrapper::~FileDialogEventsWrapper() {}

void FileDialogEventsWrapper::init(std::function<void(String)> onSelectCallback, std::function<void()> onCancelCallback) {
    _onSelect = onSelectCallback;
    _onCancel = onCancelCallback;
}

void FileDialogEventsWrapper::onSelect(String value) {
    _onSelect(value);
}

void FileDialogEventsWrapper::onCancel() {
    _onCancel();
}

// ConfirmationDialogEventsWrapper

void ConfirmationDialogEventsWrapper::_bind_methods() {
    ClassDB::bind_method(D_METHOD("onConfirm", "value"), &ConfirmationDialogEventsWrapper::onConfirm);
}

ConfirmationDialogEventsWrapper::ConfirmationDialogEventsWrapper() {}

ConfirmationDialogEventsWrapper::~ConfirmationDialogEventsWrapper() {}

void ConfirmationDialogEventsWrapper::init(std::function<void(bool)> onConfirmCallback) {
    _onConfirm = onConfirmCallback;
}

void ConfirmationDialogEventsWrapper::onConfirm(bool value) {
    _onConfirm(value);
}