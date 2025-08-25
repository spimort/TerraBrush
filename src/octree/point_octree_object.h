// <copyright file="point_octree_object.h">
//     Distributed under the BSD Licence (see LICENCE file).
//
//     Copyright (c) 2014, Nition, http://www.momentstudio.co.nz/
//     Copyright (c) 2017, Máté Cserép, http://codenet.hu
//     All rights reserved.
// </copyright>

// Modified from the original code by Nition (2014) and Máté Cserép (2017).

#ifndef POINT_OCTREE_OBJECT_H
#define POINT_OCTREE_OBJECT_H

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

/// <summary>
/// An object in the octree
/// </summary>
class PointOctreeObject : public RefCounted {
    GDCLASS(PointOctreeObject, RefCounted)

private:
    /// <summary>
    /// Object content
    /// </summary>
    Ref<RefCounted> _obj;

    /// <summary>
    /// Object position
    /// </summary>
    Vector3 _pos;

protected:
    static void _bind_methods();

public:
    PointOctreeObject();
    ~PointOctreeObject();

    /// <summary>
    /// Get the object content
    /// </summary>
    Ref<RefCounted> get_obj() const;

    /// <summary>
    /// Set the object content
    /// </summary>
    void set_obj(const Ref<RefCounted> &value);

    /// <summary>
    /// Get the object position
    /// </summary>
    Vector3 get_pos() const;

    /// <summary>
    /// Set the object position
    /// </summary>
    void set_pos(const Vector3 value);
};
#endif