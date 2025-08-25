// <copyright file="point_octree_bounding_box.h">
//     Distributed under the BSD Licence (see LICENCE file).
//
//     Copyright (c) 2014, Nition, http://www.momentstudio.co.nz/
//     Copyright (c) 2017, Máté Cserép, http://codenet.hu
//     All rights reserved.
// </copyright>

// Modified from the original code by Nition (2014) and Máté Cserép (2017).

#ifndef POINT_OCTREE_BOUNDING_BOX_H
#define POINT_OCTREE_BOUNDING_BOX_H

#include "point_octree_ray.h"

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

class PointOctreeBoundingBox : public RefCounted {
    GDCLASS(PointOctreeBoundingBox, RefCounted)

private:
        /// <summary>
        /// Gets or sets the center of the bounding box.
        /// </summary>
        Vector3 _center;

        /// <summary>
        /// Gets or sets the extents of the bounding box. This is always half of the <see cref="Size"/>.
        /// </summary>
        Vector3 _extents;

protected:
    static void _bind_methods();

public:
    PointOctreeBoundingBox();
    ~PointOctreeBoundingBox();

    /// <summary>
    /// Initialize a new bounding box.
    /// </summary>
    /// <param name="center">The center of the box.</param>
    /// <param name="size">The size of the box.</param>
    void init(Vector3 center, Vector3 size);

    /// <summary>
    /// Gets the center of the bounding box.
    /// </summary>
    Vector3 get_center() const;
    /// <summary>
    /// Sets the center of the bounding box.
    /// </summary>
    void set_center(const Vector3 value);

    /// <summary>
    /// Gets the extents of the bounding box. This is always half of the <see cref="Size"/>.
    /// </summary>
    Vector3 get_extents() const;
    /// <summary>
    /// Sets the extents of the bounding box. This is always half of the <see cref="Size"/>.
    /// </summary>
    void set_extents(const Vector3 value);

    /// <summary>
    /// Gets the size of the bounding box. This is always twice as large as the <see cref="Extents"/>.
    /// </summary>
    Vector3 get_size() const;
    /// <summary>
    /// Sets the size of the bounding box. This is always twice as large as the <see cref="Extents"/>.
    /// </summary>
    void set_size(const Vector3 value);

    /// <summary>
    /// Gets the minimal point of the box.
    /// </summary>
    /// <remarks>
    /// This is always equal to <c>center-extents</c>.
    /// </remarks>
    Vector3 get_min() const;
    /// <summary>
    /// Sets the minimal point of the box.
    /// </summary>
    /// <remarks>
    /// This is always equal to <c>center-extents</c>.
    /// </remarks>
    void set_min(const Vector3 value);

    /// <summary>
    /// Gets the maximal point of the box.
    /// </summary>
    /// <remarks>
    /// This is always equal to <c>center+extents</c>.
    /// </remarks>
    Vector3 get_max() const;
    /// <summary>
    /// Sets the maximal point of the box.
    /// </summary>
    /// <remarks>
    /// This is always equal to <c>center+extents</c>.
    /// </remarks>
    void set_max(const Vector3 value);

    /// <summary>
    /// Sets the bounds to the min and max value of the box.
    /// </summary>
    /// <param name="min">The minimal point.</param>
    /// <param name="max">The maximal point.</param>
    void setMinMax(Vector3 min, Vector3 max);

    /// <summary>
    /// Grows the bounding box include the point.
    /// </summary>
    /// <param name="point">The specified point to include.</param>
    void encapsulate(Vector3 point);

    /// <summary>
    /// Grows the bounding box include the other box.
    /// </summary>
    /// <param name="box">The specified box to include.</param>
    void encapsulate(Ref<PointOctreeBoundingBox> box);

    /// <summary>
    /// Expands the bounds by increasing its <see cref="Size"/> by <paramref name="amount"/> along each side.
    /// </summary>
    /// <param name="amount">The expansions for each dimension.</param>
    void expand(float amount);

    /// <summary>
    /// Expands the bounds by increasing its <see cref="Size"/> by <paramref name="amount"/> along each side.
    /// </summary>
    /// <param name="amount">The expansions for each dimension in order.</param>
    void expand(Vector3 amount);

    /// <summary>
    /// Determines whether the box contains the point.
    /// </summary>
    /// <param name="point">The point to test.</param>
    /// <returns><c>true</c> if the box contains the point; otherwise, <c>false</c>.</returns>
    bool contains(Vector3 point);

    /// <summary>
    /// Determines whether the bounding box intersects with another box.
    /// </summary>
    /// <param name="box">The box to test.</param>
    /// <returns><c>true</c> if the bounding box intersects with another box, <c>false</c> otherwise.</returns>
    bool intersects(Ref<PointOctreeBoundingBox> box);

    /// <summary>
    /// Determines whether the bounding box intersects with a ray.
    /// </summary>
    /// <param name="ray">The ray to test.</param>
    /// <returns><c>true</c> if the box intersects with the ray, <c>false</c> otherwise.</returns>
    bool intersectRay(Ref<PointOctreeRay> ray);

    /// <summary>
    /// Determines whether the bounding box intersects with a ray.
    /// </summary>
    /// <param name="ray">The ray to test.</param>
    /// <param name="distance">The calculated distance from the origin of the ray to the box along the ray.</param>
    /// <returns><c>true</c> if the box intersects with the ray, <c>false</c> otherwise.</returns>
    bool intersectRay(Ref<PointOctreeRay> ray, float &distance);

    /// <summary>
    /// Determines whether the specified object as a <see cref="BoundingBox" /> is equal to this instance.
    /// </summary>
    /// <param name="other">The <see cref="BoundingBox" /> object to compare with this instance.</param>
    /// <returns><c>true</c> if the specified box is equal to this instance; otherwise, <c>false</c>.</returns>
    bool equals(Ref<PointOctreeBoundingBox> other);

    /// <summary>
    /// Returns a nicely formatted string for this bounding box.
    /// </summary>
    String toString();

    /// <summary>
    /// Determines whether two bounding boxes are equal.
    /// </summary>
    /// <param name="lhs">The first box.</param>
    /// <param name="rhs">The second box.</param>
    bool operator==(const Ref<PointOctreeBoundingBox> rhs) const;

    /// <summary>
    /// Determines whether two bounding boxes are different.
    /// </summary>
    /// <param name="lhs">The first box.</param>
    /// <param name="rhs">The second box.</param>
    bool operator!=(const Ref<PointOctreeBoundingBox> rhs) const;
};
#endif