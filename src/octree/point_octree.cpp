// <copyright file="point_octree.cpp">
//     Distributed under the BSD Licence (see LICENCE file).
//
//     Copyright (c) 2014, Nition, http://www.momentstudio.co.nz/
//     Copyright (c) 2017, Máté Cserép, http://codenet.hu
//     All rights reserved.
// </copyright>

// Modified from the original code by Nition (2014) and Máté Cserép (2017).

#include "point_octree.h"

using namespace godot;

void PointOctree::_bind_methods() {}

PointOctree::PointOctree() {}

PointOctree::~PointOctree() {}

void PointOctree::init(float initialWorldSize, Vector3 initialWorldPos, float minNodeSize) {
    ERR_FAIL_COND_MSG(minNodeSize > initialWorldSize, "Minimum node size must be at least as big as the initial world size.");

    _count = 0;
    _initialSize = initialWorldSize;
    _minSize = minNodeSize;
    _rootNode = Ref<PointOctreeNode>(memnew(PointOctreeNode));
    _rootNode->init(_initialSize, _minSize, initialWorldPos);
}

void PointOctree::grow(Vector3 direction) {
    int xDirection = direction.x >= 0 ? 1 : -1;
    int yDirection = direction.y >= 0 ? 1 : -1;
    int zDirection = direction.z >= 0 ? 1 : -1;
    Ref<PointOctreeNode> oldRoot = _rootNode;
    float half = _rootNode->get_sideLength() / 2;
    float newLength = _rootNode->get_sideLength() * 2;
    Vector3 newCenter = _rootNode->get_center() + Vector3(xDirection * half, yDirection * half, zDirection * half);

    // Create a new, bigger octree root node
    _rootNode = Ref<PointOctreeNode>(memnew(PointOctreeNode));
    _rootNode->init(newLength, _minSize, newCenter);

    if (oldRoot->hasAnyObjects())
    {
        // Create 7 new octree children to go with the old root as children of the new root
        int rootPos = _rootNode->bestFitChild(oldRoot->get_center());
        TypedArray<Ref<PointOctreeNode>> children = TypedArray<Ref<PointOctreeNode>>();
        children.resize(8);
        for (int i = 0; i < 8; i++)
        {
            if (i == rootPos)
            {
                children[i] = oldRoot;
            }
            else
            {
                xDirection = i % 2 == 0 ? -1 : 1;
                yDirection = i > 3 ? -1 : 1;
                zDirection = (i < 2 || (i > 3 && i < 6)) ? -1 : 1;
                children[i] = memnew(PointOctreeNode);
                Ref<PointOctreeNode>(children[i])->init(
                    oldRoot->get_sideLength(),
                    _minSize,
                    newCenter + Vector3(xDirection * half, yDirection * half, zDirection * half));
            }
        }

        // Attach the new children to the new root node
        _rootNode->setChildren(children);
    }
}

void PointOctree::shrink() {
    _rootNode = _rootNode->shrinkIfPossible(_initialSize);
}

int PointOctree::size() {
    return _count;
}

Ref<PointOctreeBoundingBox> PointOctree::maxBounds() {
    Ref<PointOctreeBoundingBox> boundingBox = memnew(PointOctreeBoundingBox);
    boundingBox->init(_rootNode->get_center(), Vector3(_rootNode->get_sideLength(), _rootNode->get_sideLength(), _rootNode->get_sideLength()));
    return boundingBox;
}

TypedArray<Ref<PointOctreeBoundingBox>> PointOctree::getChildBounds() {
    TypedArray<Ref<PointOctreeBoundingBox>> bounds = TypedArray<Ref<PointOctreeBoundingBox>>();
    _rootNode->getChildBounds(bounds);
    return bounds;
}

void PointOctree::add(Ref<RefCounted> obj, Vector3 objPos) {
    // Add object or expand the octree until it can be added
    int count = 0; // Safety check against infinite/excessive growth
    while (!_rootNode->add(obj, objPos))
    {
        grow(objPos - _rootNode->get_center());
        ERR_FAIL_COND_MSG(++count > 20, "Aborted Add operation as it seemed to be going on forever (" + String::num_int64(count - 1) +  " attempts at growing the octree).");
    }
    _count++;
}

bool PointOctree::remove(Ref<RefCounted> obj) {
    bool removed = _rootNode->remove(obj);

    // See if we can shrink the octree down now that we've removed the item
    if (removed)
    {
        _count--;
        shrink();
    }

    return removed;
}

bool PointOctree::remove(Ref<RefCounted> obj, Vector3 objPos) {
    bool removed = _rootNode->remove(obj, objPos);

    // See if we can shrink the octree down now that we've removed the item
    if (removed)
    {
        _count--;
        shrink();
    }

    return removed;
}

TypedArray<Ref<RefCounted>> PointOctree::getNearby(Ref<PointOctreeRay> ray, float maxDistance) {
    TypedArray<Ref<RefCounted>> collidingWith = TypedArray<Ref<RefCounted>>();
    _rootNode->getNearby(ray, maxDistance, collidingWith);
    return collidingWith;
}

TypedArray<Ref<RefCounted>> PointOctree::getNearby(Vector3 position, float maxDistance) {
    TypedArray<Ref<RefCounted>> collidingWith = TypedArray<Ref<RefCounted>>();
    _rootNode->getNearby(position, maxDistance, collidingWith);
    return collidingWith;
}

bool PointOctree::getNearbyNonAlloc(Ref<PointOctreeRay> ray, float maxDistance, TypedArray<Ref<RefCounted>> nearby) {
    nearby.clear();
    _rootNode->getNearby(ray, maxDistance, nearby);
    return nearby.size() > 0;
}

bool PointOctree::getNearbyNonAlloc(Vector3 position, float maxDistance, TypedArray<Ref<RefCounted>> nearby) {
    nearby.clear();
    _rootNode->getNearby(position, maxDistance, nearby);
    return nearby.size() > 0;
}

TypedArray<Ref<RefCounted>> PointOctree::getAll() {
    TypedArray<Ref<RefCounted>> objects = TypedArray<Ref<RefCounted>>();
    _rootNode->getAll(objects);
    return objects;
}