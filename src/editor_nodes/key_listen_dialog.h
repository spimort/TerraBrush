#ifndef KEY_LISTEN_DIALOG_H
#define KEY_LISTEN_DIALOG_H

#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/label.hpp>

#include <functional>

using namespace godot;

class KeyListenDialog : public Window {
    GDCLASS(KeyListenDialog, Window);

private:
    Button *_okButton = nullptr;
    Label *_keyDisplay = nullptr;

    Ref<InputEventKey> _eventKey = nullptr;

    std::function<void(Ref<InputEventKey>)> _onAccept = nullptr;

    void onOkButton();
    void onCancelButton();

protected:
    static void _bind_methods();

public:
    KeyListenDialog();
    ~KeyListenDialog();
    void init(std::function<void(Ref<InputEventKey>)> onAccept);

    void _ready() override;
    void _input(const Ref<InputEvent> &event) override;
};
#endif