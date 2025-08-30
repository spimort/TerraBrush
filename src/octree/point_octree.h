// <copyright file="point_octree.h">
//     Distributed under the BSD Licence (see LICENCE file).
//
//     Copyright (c) 2014, Nition, http://www.momentstudio.co.nz/
//     Copyright (c) 2017, Máté Cserép, http://codenet.hu
//     All rights reserved.
// </copyright>

// Modified from the original code by Nition (2014) and Máté Cserép (2017).

#ifndef POINT_OCTREE_H
#define POINT_OCTREE_H

#include "point_octree_node.h"
#include "point_octree_bounding_box.h"
#include "point_octree_ray.h"

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

class PointOctree : public RefCounted {
    GDCLASS(PointOctree, RefCounted);

private:
    /// <summary>
    /// Root node of the octree
    /// </summary>
    Ref<PointOctreeNode> _rootNode;

    /// <summary>
    /// Size that the octree was on creation
    /// </summary>
    float _initialSize;

    /// <summary>
    /// Minimum side length that a node can be - essentially an alternative to having a max depth
    /// </summary>
    float _minSize;

    /// <summary>
    /// The total amount of objects currently in the tree
    /// </summary>
    int _count;

    /// <summary>
    /// Grow the octree to fit in all objects.
    /// </summary>
    /// <param name="direction">Direction to grow.</param>
    void grow(Vector3 direction);

    /// <summary>
    /// Shrink the octree if possible, else leave it the same.
    /// </summary>
    void shrink();

protected:
    static void _bind_methods();

public:
    PointOctree();
    ~PointOctree();

    /// <summary>
    /// Initialize the octree
    /// </summary>
    /// <param name="initialWorldSize">Size of the sides of the initial node. The octree will never shrink smaller than this.</param>
    /// <param name="initialWorldPos">Position of the centre of the initial node.</param>
    /// <param name="minNodeSize">Nodes will stop splitting if the new nodes would be smaller than this.</param>
    /// <exception cref="ArgumentException">Minimum node size must be at least as big as the initial world size.</exception>
    void init(float initialWorldSize, Vector3 initialWorldPos, float minNodeSize);

    /// <summary>
    /// The total amount of objects currently in the tree
    /// </summary>
    int size();

    /// <summary>
    /// Gets the bounding box that represents the whole octree
    /// </summary>
    /// <value>The bounding box of the root node.</value>
    Ref<PointOctreeBoundingBox> maxBounds();

    /// <summary>
    /// Gets All the bounding box that represents the whole octree
    /// </summary>
    /// <returns></returns>
    TypedArray<Ref<PointOctreeBoundingBox>> getChildBounds();

    /// <summary>
    /// Add an object.
    /// </summary>
    /// <param name="obj">Object to add.</param>
    /// <param name="objPos">Position of the object.</param>
    /// <exception cref="InvalidOperationException">Add operation required growing the octree too much.</exception>
    void add(Ref<RefCounted> obj, Vector3 objPos);

    /// <summary>
    /// Remove an object. Makes the assumption that the object only exists once in the tree.
    /// </summary>
    /// <param name="obj">Object to remove.</param>
    /// <returns>True if the object was removed successfully.</returns>
    bool remove(Ref<RefCounted> obj);

    /// <summary>
    /// Removes the specified object at the given position. Makes the assumption that the object only exists once in the tree.
    /// </summary>
    /// <param name="obj">Object to remove.</param>
    /// <param name="objPos">Position of the object.</param>
    /// <returns>True if the object was removed successfully.</returns>
    bool remove(Ref<RefCounted> obj, Vector3 objPos);

    /// <summary>
    /// Returns objects that are within <paramref name="maxDistance"/> of the specified ray.
    /// If none, returns an empty array (not null).
    /// </summary>
    /// <param name="ray">The ray. Passing as ref to improve performance since it won't have to be copied.</param>
    /// <param name="maxDistance">Maximum distance from the ray to consider.</param>
    /// <returns>Objects within range.</returns>
    TypedArray<Ref<RefCounted>> getNearby(Ref<PointOctreeRay> ray, float maxDistance);

    /// <summary>
    /// Returns objects that are within <paramref name="maxDistance"/> of the specified position.
    /// If none, returns an empty array (not null).
    /// </summary>
    /// <param name="position">The position. Passing as ref to improve performance since it won't have to be copied.</param>
    /// <param name="maxDistance">Maximum distance from the position to consider.</param>
    /// <returns>Objects within range.</returns>
    TypedArray<Ref<RefCounted>> getNearby(Vector3 position, float maxDistance);

    /// <summary>
    /// Returns objects that are within <paramref name="maxDistance"/> of the specified ray.
    /// If none, returns false. Uses supplied list for results.
    /// </summary>
    /// <param name="ray">The ray. Passing as ref to improve performance since it won't have to be copied.</param>
    /// <param name="maxDistance">Maximum distance from the ray to consider.</param>
    /// <param name="nearby">Pre-initialized list to populate.</param>
    /// <returns><c>true</c> if items are found, <c>false</c> otherwise.</returns>
    bool getNearbyNonAlloc(Ref<PointOctreeRay> ray, float maxDistance, TypedArray<Ref<RefCounted>> nearby);

    /// <summary>
    /// Returns objects that are within <paramref name="maxDistance"/> of the specified position.
    /// If none, returns false. Uses supplied list for results.
    /// </summary>
    /// <param name="position">The position. Passing as ref to improve performance since it won't have to be copied.</param>
    /// <param name="maxDistance">Maximum distance from the position to consider.</param>
    /// <param name="nearby">Pre-initialized list to populate.</param>
    /// <returns><c>true</c> if items are found, <c>false</c> otherwise.</returns>
    bool getNearbyNonAlloc(Vector3 position, float maxDistance, TypedArray<Ref<RefCounted>> nearby);

    /// <summary>
    /// Returns all objects in the tree.
    /// If none, returns an empty array (not null).
    /// </summary>
    /// <returns>All objects.</returns>
    TypedArray<Ref<RefCounted>> getAll();
};
#endif