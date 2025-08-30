// <copyright file="point_octree_ray.h">
//     Distributed under the BSD Licence (see LICENCE file).
//
//     Copyright (c) 2014, Nition, http://www.momentstudio.co.nz/
//     Copyright (c) 2017, Máté Cserép, http://codenet.hu
//     All rights reserved.
// </copyright>

// Modified from the original code by Nition (2014) and Máté Cserép (2017).

#ifndef POINT_OCTREE_RAY_H
#define POINT_OCTREE_RAY_H

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

class PointOctreeRay : public RefCounted {
    GDCLASS(PointOctreeRay, RefCounted);

private:
    /// <summary>
    /// Gets or sets the origin of the ray.
    /// </summary>
    Vector3 _origin;

    /// <summary>
    /// The direction of the ray.
    /// </summary>
    Vector3 _direction;

protected:
    static void _bind_methods();

public:
    PointOctreeRay();
    ~PointOctreeRay();

    /// <summary>
    /// Initialize a ray starting at origin along direction.
    /// </summary>
    /// <param name="origin">The origin of the ray.</param>
    /// <param name="direction">The direction of the ray.</param>
    void init(Vector3 origin, Vector3 direction);

    /// <summary>
    /// Gets the origin of the ray.
    /// </summary>
    Vector3 get_origin() const;
    /// <summary>
    /// Sets the origin of the ray.
    /// </summary>
    void set_origin(const Vector3 value);

    /// <summary>
    /// Gets the direction of the ray.
    /// </summary>
    Vector3 get_direction() const;

    /// <summary>
    /// Sets the direction of the ray.
    /// </summary>
    void set_direction(const Vector3 value);

    /// <summary>
    /// Returns a point at the given distance along the ray.
    /// </summary>
    /// <param name="distance">The distance.</param>
    /// <returns>The point on the ray.</returns>
    Vector3 getPoint(float distance);

    /// <summary>
    /// Returns a nicely formatted string for this ray.
    /// </summary>
    String toString();
};
#endif