// <copyright file="point_octree_node.h">
//     Distributed under the BSD Licence (see LICENCE file).
//
//     Copyright (c) 2014, Nition, http://www.momentstudio.co.nz/
//     Copyright (c) 2017, Máté Cserép, http://codenet.hu
//     All rights reserved.
// </copyright>

// Modified from the original code by Nition (2014) and Máté Cserép (2017).

#ifndef POINT_OCTREE_NODE_H
#define POINT_OCTREE_NODE_H

#include "point_octree_object.h"
#include "point_octree_bounding_box.h"
#include "point_octree_ray.h"

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

class PointOctreeNode : public RefCounted {
    GDCLASS(PointOctreeNode, RefCounted);

private:
    /// <summary>
    /// If there are already NumObjectsAllowed in a node, we split it into children
    /// </summary>
    /// <remarks>
    /// A generally good number seems to be something around 8-15
    /// </remarks>
    const int NumObjectsAllowed = 8;

    /// <summary>
    /// Center of this node
    /// </summary>
    Vector3 _center = Vector3();

    /// <summary>
    /// Length of the sides of this node
    /// </summary>
    float _sideLength = 0;

    /// <summary>
    /// Minimum size for a node in this octree
    /// </summary>
    float _minSize = 0;

    /// <summary>
    /// Bounding box that represents this node
    /// </summary>
    Ref<PointOctreeBoundingBox> _bounds = nullptr;

    /// <summary>
    /// Objects in this node
    /// </summary>
    TypedArray<Ref<PointOctreeObject>> _objects = TypedArray<Ref<PointOctreeObject>>();

    /// <summary>
    /// Child nodes, if any
    /// </summary>
    TypedArray<Ref<PointOctreeNode>> _children = TypedArray<Ref<PointOctreeNode>>();

    /// <summary>
    /// Bounds of potential children to this node. These are actual size (with looseness taken into account), not base size
    /// </summary>
    TypedArray<Ref<PointOctreeBoundingBox>> _childBounds = TypedArray<Ref<PointOctreeBoundingBox>>();

    /// <summary>
    /// For reverting the bounds size after temporary changes
    /// </summary>
    Vector3 _actualBoundsSize = Vector3();

    /// <summary>
    /// Gets a value indicating whether this node has children
    /// </summary>
    bool hasChildren();

    /// <summary>
    /// Set values for this node.
    /// </summary>
    /// <param name="baseLengthVal">Length of this node, not taking looseness into account.</param>
    /// <param name="minSizeVal">Minimum size of nodes in this octree.</param>
    /// <param name="centerVal">Centre position of this node.</param>
    void setValues(float baseLengthVal, float minSizeVal, Vector3 centerVal);

    /// <summary>
    /// Private counterpart to the public Add method.
    /// </summary>
    /// <param name="obj">Object to add.</param>
    /// <param name="objPos">Position of the object.</param>
    void subAdd(Ref<RefCounted> obj, Vector3 objPos);

    /// <summary>
    /// Private counterpart to the public <see cref="Remove(T, Vector3)"/> method.
    /// </summary>
    /// <param name="obj">Object to remove.</param>
    /// <param name="objPos">Position of the object.</param>
    /// <returns>True if the object was removed successfully.</returns>
    bool subRemove(Ref<RefCounted> obj, Vector3 objPos);

    /// <summary>
    /// Splits the octree into eight children.
    /// </summary>
    void split();

    /// <summary>
    /// Merge all children into this node - the opposite of Split.
    /// Note: We only have to check one level down since a merge will never happen if the children already have children,
    /// since THAT won't happen unless there are already too many objects to merge.
    /// </summary>
    void merge();

    /// <summary>
    /// Checks if outerBounds encapsulates the given point.
    /// </summary>
    /// <param name="outerBounds">Outer bounds.</param>
    /// <param name="point">Point.</param>
    /// <returns>True if innerBounds is fully encapsulated by outerBounds.</returns>
    static bool encapsulates(Ref<PointOctreeBoundingBox> outerBounds, Vector3 point);

    /// <summary>
    /// Checks if there are few enough objects in this node and its children that the children should all be merged into this.
    /// </summary>
    /// <returns>True there are less or the same amount of objects in this and its children than <see cref="NumObjectsAllowed"/>.</returns>
    bool shouldMerge();

protected:
    static void _bind_methods();

public:
    PointOctreeNode();
    ~PointOctreeNode();

    /// <summary>
    /// Initialized.
    /// </summary>
    /// <param name="baseLengthVal">Length of this node, not taking looseness into account.</param>
    /// <param name="minSizeVal">Minimum size of nodes in this octree.</param>
    /// <param name="centerVal">Center position of this node.</param>
    void init(float baseLengthVal, float minSizeVal, Vector3 centerVal);

    /// <summary>
    /// Gets the center that represents this node
    /// </summary>
    Vector3 get_center() const;

    /// <summary>
    /// Gets the side length that represents this node
    /// </summary>
    float get_sideLength() const;

    /// <summary>
    /// Gets the bounding box that represents this node
    /// </summary>
    Ref<PointOctreeBoundingBox> bounds();

    /// <summary>
    /// Gets All the bounding box that represents this node
    /// </summary>
    /// <param name="bounds"></param>
    void getChildBounds(TypedArray<Ref<PointOctreeBoundingBox>> bounds);

    /// <summary>
    /// Add an object.
    /// </summary>
    /// <param name="obj">Object to add.</param>
    /// <param name="objPos">Position of the object.</param>
    /// <returns></returns>
    bool add(Ref<RefCounted> obj, Vector3 objPos);

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
    /// Return objects that are within <paramref name="maxDistance"/> of the specified ray.
    /// </summary>
    /// <param name="ray">The ray.</param>
    /// <param name="maxDistance">Maximum distance from the ray to consider.</param>
    /// <param name="result">List result.</param>
    /// <returns>Objects within range.</returns>
    void getNearby(Ref<PointOctreeRay> ray, float maxDistance, TypedArray<Ref<RefCounted>> result);

    /// <summary>
    /// Return objects that are within <paramref name="maxDistance"/> of the specified position.
    /// </summary>
    /// <param name="position">The position.</param>
    /// <param name="maxDistance">Maximum distance from the position to consider.</param>
    /// <param name="result">List result.</param>
    /// <returns>Objects within range.</returns>
    void getNearby(Vector3 &position, float maxDistance, TypedArray<Ref<RefCounted>> result);

    /// <summary>
    /// Return all objects in the tree.
    /// </summary>
    /// <returns>All objects.</returns>
    void getAll(TypedArray<Ref<RefCounted>> result);

    /// <summary>
    /// Set the 8 children of this octree.
    /// </summary>
    /// <param name="childOctrees">The 8 new child nodes.</param>
    void setChildren(TypedArray<Ref<PointOctreeNode>> childOctrees);

    /// <summary>
    /// We can shrink the octree if:
    /// - This node is >= double minLength in length
    /// - All objects in the root node are within one octant
    /// - This node doesn't have children, or does but 7/8 children are empty
    /// We can also shrink it if there are no objects left at all!
    /// </summary>
    /// <param name="minLength">Minimum dimensions of a node in this octree.</param>
    /// <returns>The new root, or the existing one if we didn't shrink.</returns>
    Ref<PointOctreeNode> shrinkIfPossible(float minLength);

    /// <summary>
    /// Find which child node this object would be most likely to fit in.
    /// </summary>
    /// <param name="objPos">The object's position.</param>
    /// <returns>One of the eight child octants.</returns>
    int bestFitChild(Vector3 objPos);

    /// <summary>
    /// Checks if this node or anything below it has something in it.
    /// </summary>
    /// <returns>True if this node or any of its children, grandchildren etc have something in them</returns>
    bool hasAnyObjects();

    /// <summary>
    /// Returns the squared distance to the given ray from a point.
    /// </summary>
    /// <param name="ray">The ray.</param>
    /// <param name="point">The point to check distance from the ray.</param>
    /// <returns>Squared distance from the point to the closest point of the ray.</returns>
    static float sqrDistanceToRay(Ref<PointOctreeRay> ray, Vector3 point);
};
#endif