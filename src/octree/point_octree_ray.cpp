// <copyright file="point_octree_ray.cpp">
//     Distributed under the BSD Licence (see LICENCE file).
//
//     Copyright (c) 2014, Nition, http://www.momentstudio.co.nz/
//     Copyright (c) 2017, Máté Cserép, http://codenet.hu
//     All rights reserved.
// </copyright>

// Modified from the original code by Nition (2014) and Máté Cserép (2017).

#include "point_octree_ray.h"

using namespace godot;

void PointOctreeRay::_bind_methods() {}

PointOctreeRay::PointOctreeRay() {
    _origin = Vector3();
    _direction = Vector3();
}

PointOctreeRay::~PointOctreeRay() {}

void PointOctreeRay::init(Vector3 origin, Vector3 direction) {
    _origin = origin;
    _direction = direction.normalized();
}

Vector3 PointOctreeRay::get_origin() const {
    return _origin;
}
void PointOctreeRay::set_origin(const Vector3 value) {
    _origin = value;
}

Vector3 PointOctreeRay::get_direction() const {
    return _direction;
}
void PointOctreeRay::set_direction(const Vector3 value) {
    _direction = value.normalized();
}

Vector3 PointOctreeRay::getPoint(float distance) {
     return _origin + _direction * distance;
}

String PointOctreeRay::toString() {
    return String("Origin: {0}, Dir: {1}").format(Array::make(_origin, _direction));
}