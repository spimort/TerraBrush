// <copyright file="point_octree_object.cpp">
//     Distributed under the BSD Licence (see LICENCE file).
//
//     Copyright (c) 2014, Nition, http://www.momentstudio.co.nz/
//     Copyright (c) 2017, Máté Cserép, http://codenet.hu
//     All rights reserved.
// </copyright>

// Modified from the original code by Nition (2014) and Máté Cserép (2017).

#include "point_octree_object.h"

using namespace godot;

void PointOctreeObject::_bind_methods() {}

PointOctreeObject::PointOctreeObject() {}

PointOctreeObject::~PointOctreeObject() {}

Ref<RefCounted> PointOctreeObject::get_obj() const {
    return _obj;
}
void PointOctreeObject::set_obj(const Ref<RefCounted> &value) {
    _obj = value;
}

Vector3 PointOctreeObject::get_pos() const {
    return _pos;
}
void PointOctreeObject::set_pos(const Vector3 value) {
    _pos = value;
}