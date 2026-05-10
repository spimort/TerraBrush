#ifndef THREAD_UTILS_H
#define THREAD_UTILS_H

#include <godot_cpp/classes/ref.hpp>

using namespace godot;

class CancellationToken : public RefCounted {
    GDCLASS(CancellationToken, RefCounted);

private:
    bool _isCancellationRequested = false;

protected:
    static void _bind_methods();

public:
    CancellationToken();
    ~CancellationToken();

    bool isCancellationRequested() const;
    void requestCancel();
};

class CancellationSource : public RefCounted {
    GDCLASS(CancellationSource, RefCounted);

private:
    Ref<CancellationToken> _token = nullptr;

protected:
    static void _bind_methods();

public:
    CancellationSource();
    ~CancellationSource();

    void cancel();
    Ref<CancellationToken> getToken() const;
};
#endif