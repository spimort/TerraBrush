#ifndef HASH_UTILS_MANAGER_H
#define HASH_UTILS_MANAGER_H

#include <godot_cpp/classes/ref.hpp>

namespace std {
    template<typename T>
    struct hash<godot::Ref<T>> {
        size_t operator()(const godot::Ref<T>& ref) const {
            return std::hash<T*>()(ref.ptr());
        }
    };
}
#endif