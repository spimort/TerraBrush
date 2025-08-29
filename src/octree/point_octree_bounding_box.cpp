// <copyright file="point_octree_bounding_box.cpp">
//     Distributed under the BSD Licence (see LICENCE file).
//
//     Copyright (c) 2014, Nition, http://www.momentstudio.co.nz/
//     Copyright (c) 2017, Máté Cserép, http://codenet.hu
//     All rights reserved.
// </copyright>

// Modified from the original code by Nition (2014) and Máté Cserép (2017).

#include "point_octree_bounding_box.h"

using namespace godot;

void PointOctreeBoundingBox::_bind_methods() {}

PointOctreeBoundingBox::PointOctreeBoundingBox() {
    _center = Vector3(0, 0, 0);
    _extents = Vector3(0, 0, 0);
}

PointOctreeBoundingBox::~PointOctreeBoundingBox() {}

void PointOctreeBoundingBox::init(Vector3 center, Vector3 size) {
    _center = center;
    _extents = size * 0.5;
}

Vector3 PointOctreeBoundingBox::get_center() const {
    return _center;
}
void PointOctreeBoundingBox::set_center(const Vector3 value) {
    _center = value;
}

Vector3 PointOctreeBoundingBox::get_extents() const {
    return _extents;
}
void PointOctreeBoundingBox::set_extents(const Vector3 value) {
    _extents = value;
}

Vector3 PointOctreeBoundingBox::get_size() const {
    return _extents * 2;
}
void PointOctreeBoundingBox::set_size(const Vector3 value) {
    _extents = value * 0.5;
}

Vector3 PointOctreeBoundingBox::get_min() const {
    return _center - _extents;
}
void PointOctreeBoundingBox::set_min(const Vector3 value) {
    setMinMax(value, get_max());
}

Vector3 PointOctreeBoundingBox::get_max() const {
    return _center + _extents;
}
void PointOctreeBoundingBox::set_max(const Vector3 value) {
    setMinMax(get_min(), value);
}

void PointOctreeBoundingBox::setMinMax(Vector3 min, Vector3 max) {
    _extents = (max - min) * 0.5;
    _center = min + _extents;
}

void PointOctreeBoundingBox::encapsulate(Vector3 point) {
    setMinMax(point.min(get_min()), point.max(get_max()));
}

void PointOctreeBoundingBox::encapsulate(Ref<PointOctreeBoundingBox> box) {
    encapsulate(box->get_center() - box->get_extents());
    encapsulate(box->get_center() + box->get_extents());
}

void PointOctreeBoundingBox::expand(float amount) {
    amount *= 0.5;
    set_extents(get_extents() + Vector3(amount, amount, amount));
}

void PointOctreeBoundingBox::expand(Vector3 amount) {
    set_extents(get_extents() + (amount * 0.5));
}

bool PointOctreeBoundingBox::contains(Vector3 point) {
    return
        get_min().x <= point.x && get_max().x >= point.x &&
        get_min().y <= point.y && get_max().y >= point.y &&
        get_min().z <= point.z && get_max().z >= point.z;
}

bool PointOctreeBoundingBox::intersects(Ref<PointOctreeBoundingBox> box) {
    return
        get_min().x <= box->get_max().x && get_max().x >= box->get_min().x &&
        get_min().y <= box->get_max().y && get_max().y >= box->get_min().y &&
        get_min().z <= box->get_max().z && get_max().z >= box->get_min().z;
}

bool PointOctreeBoundingBox::intersectRay(Ref<PointOctreeRay> ray) {
    float distance;
    return intersectRay(ray, distance);
}

bool PointOctreeBoundingBox::intersectRay(Ref<PointOctreeRay> ray, float &distance) {
    Vector3 dirFrac = Vector3(
        1 / ray->get_direction().x,
        1 / ray->get_direction().y,
        1 / ray->get_direction().z
    );

    float t1 = (get_min().x - ray->get_origin().x) * dirFrac.x;
    float t2 = (get_max().x - ray->get_origin().x) * dirFrac.x;
    float t3 = (get_min().y - ray->get_origin().y) * dirFrac.y;
    float t4 = (get_max().y - ray->get_origin().y) * dirFrac.y;
    float t5 = (get_min().z - ray->get_origin().z) * dirFrac.z;
    float t6 = (get_max().z - ray->get_origin().z) * dirFrac.z;

    float tmin = Math::max(Math::max(Math::min(t1, t2), Math::min(t3, t4)), Math::min(t5, t6));
    float tmax = Math::min(Math::min(Math::max(t1, t2), Math::max(t3, t4)), Math::max(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
    if (tmax < 0)
    {
        distance = tmax;
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        distance = tmax;
        return false;
    }

    distance = tmin;
    return true;
}

bool PointOctreeBoundingBox::equals(Ref<PointOctreeBoundingBox> other) {
    return (_center == other->get_center()) && (_extents == other->get_extents());;
}

String PointOctreeBoundingBox::toString() {
    return String("Center: {0}, Extents: {1}").format(Array::make(_center, _extents));
}

bool PointOctreeBoundingBox::operator==(const Ref<PointOctreeBoundingBox> rhs) const {
    return rhs->get_center() == _center && rhs->get_extents() == _extents;
}

bool PointOctreeBoundingBox::operator!=(const Ref<PointOctreeBoundingBox> rhs) const {
    return !(rhs == this);
}