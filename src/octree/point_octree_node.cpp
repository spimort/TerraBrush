// <copyright file="point_octree_node.cpp">
//     Distributed under the BSD Licence (see LICENCE file).
//
//     Copyright (c) 2014, Nition, http://www.momentstudio.co.nz/
//     Copyright (c) 2017, Máté Cserép, http://codenet.hu
//     All rights reserved.
// </copyright>

// Modified from the original code by Nition (2014) and Máté Cserép (2017).

#include "point_octree_node.h"
#include "point_octree_object.h"

#include <algorithm>

using namespace godot;

void PointOctreeNode::_bind_methods() {}

PointOctreeNode::PointOctreeNode() {
    _center = Vector3();
    _sideLength = 0;
    _minSize = 0;
    _bounds = Ref<PointOctreeBoundingBox>(nullptr);
    _objects = std::vector<Ref<PointOctreeObject>>();
    _children = std::vector<Ref<PointOctreeNode>>();
    _childBounds = std::vector<Ref<PointOctreeBoundingBox>>();
    _actualBoundsSize = Vector3();
}

PointOctreeNode::~PointOctreeNode() {}

void PointOctreeNode::init(float baseLengthVal, float minSizeVal, Vector3 centerVal) {
    setValues(baseLengthVal, minSizeVal, centerVal);
}

Vector3 PointOctreeNode::get_center() const {
    return _center;
}

float PointOctreeNode::get_sideLength() const {
    return _sideLength;
}

bool PointOctreeNode::hasChildren() {
    return _children.size() > 0;
}

void PointOctreeNode::setValues(float baseLengthVal, float minSizeVal, Vector3 centerVal) {
    _sideLength = baseLengthVal;
    _minSize = minSizeVal;
    _center = centerVal;

    // Create the bounding box.
    _actualBoundsSize = Vector3(_sideLength, _sideLength, _sideLength);
    _bounds = Ref<PointOctreeBoundingBox>(memnew(PointOctreeBoundingBox));
    _bounds->init(_center, _actualBoundsSize);

    float quarter = _sideLength / 4;
    float childActualLength = _sideLength / 2;
    Vector3 childActualSize = Vector3(childActualLength, childActualLength, childActualLength);
    _childBounds = std::vector<Ref<PointOctreeBoundingBox>>();
    _childBounds.resize(8);
    _childBounds[0] = Ref<PointOctreeBoundingBox>(memnew(PointOctreeBoundingBox));
    Ref<PointOctreeBoundingBox>(_childBounds[0])->init(_center + Vector3(-quarter, quarter, -quarter), childActualSize);
    _childBounds[1] = Ref<PointOctreeBoundingBox>(memnew(PointOctreeBoundingBox));
    Ref<PointOctreeBoundingBox>(_childBounds[1])->init(_center + Vector3(quarter, quarter, -quarter), childActualSize);
    _childBounds[2] = Ref<PointOctreeBoundingBox>(memnew(PointOctreeBoundingBox));
    Ref<PointOctreeBoundingBox>(_childBounds[2])->init(_center + Vector3(-quarter, quarter, quarter), childActualSize);
    _childBounds[3] = Ref<PointOctreeBoundingBox>(memnew(PointOctreeBoundingBox));
    Ref<PointOctreeBoundingBox>(_childBounds[3])->init(_center + Vector3(quarter, quarter, quarter), childActualSize);
    _childBounds[4] = Ref<PointOctreeBoundingBox>(memnew(PointOctreeBoundingBox));
    Ref<PointOctreeBoundingBox>(_childBounds[4])->init(_center + Vector3(-quarter, -quarter, -quarter), childActualSize);
    _childBounds[5] = Ref<PointOctreeBoundingBox>(memnew(PointOctreeBoundingBox));
    Ref<PointOctreeBoundingBox>(_childBounds[5])->init(_center + Vector3(quarter, -quarter, -quarter), childActualSize);
    _childBounds[6] = Ref<PointOctreeBoundingBox>(memnew(PointOctreeBoundingBox));
    Ref<PointOctreeBoundingBox>(_childBounds[6])->init(_center + Vector3(-quarter, -quarter, quarter), childActualSize);
    _childBounds[7] = Ref<PointOctreeBoundingBox>(memnew(PointOctreeBoundingBox));
    Ref<PointOctreeBoundingBox>(_childBounds[7])->init(_center + Vector3(quarter, -quarter, quarter), childActualSize);
}

void PointOctreeNode::subAdd(Ref<RefCounted> obj, Vector3 objPos) {
    // We know it fits at this level if we've got this far

    // We always put things in the deepest possible child
    // So we can skip checks and simply move down if there are children aleady
    if (!hasChildren())
    {
        // Just add if few objects are here, or children would be below min size
        if (_objects.size() < NumObjectsAllowed || (_sideLength / 2) < _minSize)
        {
            Ref<PointOctreeObject> newObj = memnew(PointOctreeObject);
            newObj->set_obj(obj);
            newObj->set_pos(objPos);
            _objects.push_back(newObj);
            return; // We're done. No children yet
        }

        // Enough objects in this node already: Create the 8 children
        int bestFitChildIndex;
        if (_children.size() == 0)
        {
            split();
            ERR_FAIL_COND_MSG(_children.size() == 0, "Child creation failed for an unknown reason. Early exit.");

            // Now that we have the new children, move this node's existing objects into them
            for (int i = _objects.size() - 1; i >= 0; i--)
            {
                Ref<PointOctreeObject> existingObj = _objects[i];
                // Find which child the object is closest to based on where the
                // object's center is located in relation to the octree's center
                bestFitChildIndex = bestFitChild(existingObj->get_pos());
                Ref<PointOctreeNode>(_children[bestFitChildIndex])->subAdd(existingObj->get_obj(), existingObj->get_pos()); // Go a level deeper
                _objects.erase(std::remove(_objects.begin(), _objects.end(), existingObj), _objects.end()); // Remove from here
            }
        }
    }

    // Handle the new object we're adding now
    int bestFit = bestFitChild(objPos);
    Ref<PointOctreeNode>(_children[bestFit])->subAdd(obj, objPos);
}

bool PointOctreeNode::subRemove(Ref<RefCounted> obj, Vector3 objPos) {
    bool removed = false;

    for (int i = 0; i < _objects.size(); i++)
    {
        if (Ref<PointOctreeObject>(_objects[i])->get_obj() == obj)
        {
            removed = true;
            _objects.erase(_objects.begin() + i);
            break;
        }
    }

    if (!removed && _children.size() > 0)
    {
        int bestFitChildIndex = bestFitChild(objPos);
        removed = Ref<PointOctreeNode>(_children[bestFitChildIndex])->subRemove(obj, objPos);
    }

    if (removed && _children.size() > 0)
    {
        // Check if we should merge nodes now that we've removed an item
        if (shouldMerge())
        {
            merge();
        }
    }

    return removed;
}

void PointOctreeNode::split() {
    float quarter = _sideLength / 4;
    float newLength = _sideLength / 2;
    _children = std::vector<Ref<PointOctreeNode>>();
    _children.resize(8);
    _children[0] = Ref<PointOctreeNode>(memnew(PointOctreeNode));
    Ref<PointOctreeNode>(_children[0])->init(newLength, _minSize, _center + Vector3(-quarter, quarter, -quarter));
    _children[1] = Ref<PointOctreeNode>(memnew(PointOctreeNode));
    Ref<PointOctreeNode>(_children[1])->init(newLength, _minSize, _center + Vector3(quarter, quarter, -quarter));
    _children[2] = Ref<PointOctreeNode>(memnew(PointOctreeNode));
    Ref<PointOctreeNode>(_children[2])->init(newLength, _minSize, _center + Vector3(-quarter, quarter, quarter));
    _children[3] = Ref<PointOctreeNode>(memnew(PointOctreeNode));
    Ref<PointOctreeNode>(_children[3])->init(newLength, _minSize, _center + Vector3(quarter, quarter, quarter));
    _children[4] = Ref<PointOctreeNode>(memnew(PointOctreeNode));
    Ref<PointOctreeNode>(_children[4])->init(newLength, _minSize, _center + Vector3(-quarter, -quarter, -quarter));
    _children[5] = Ref<PointOctreeNode>(memnew(PointOctreeNode));
    Ref<PointOctreeNode>(_children[5])->init(newLength, _minSize, _center + Vector3(quarter, -quarter, -quarter));
    _children[6] = Ref<PointOctreeNode>(memnew(PointOctreeNode));
    Ref<PointOctreeNode>(_children[6])->init(newLength, _minSize, _center + Vector3(-quarter, -quarter, quarter));
    _children[7] = Ref<PointOctreeNode>(memnew(PointOctreeNode));
    Ref<PointOctreeNode>(_children[7])->init(newLength, _minSize, _center + Vector3(quarter, -quarter, quarter));
}

void PointOctreeNode::merge() {
    // Note: We know children != null or we wouldn't be merging
    for (int i = 0; i < 8; i++)
    {
        Ref<PointOctreeNode> curChild = _children[i];
        int numObjects = curChild->_objects.size();
        for (int j = numObjects - 1; j >= 0; j--)
        {
            Ref<PointOctreeObject> curObj = curChild->_objects[j];
            _objects.push_back(curObj);
        }
    }
    // Remove the child nodes (and the objects in them - they've been added elsewhere now)
    _children.clear();
}

bool PointOctreeNode::encapsulates(Ref<PointOctreeBoundingBox> outerBounds, Vector3 point) {
    return outerBounds->contains(point);
}

bool PointOctreeNode::shouldMerge() {
    int totalObjects = _objects.size();
    if (_children.size() > 0)
    {
        for (Ref<PointOctreeNode> child : _children)
        {
            if (child->_children.size() > 0)
            {
                // If any of the *children* have children, there are definitely too many to merge,
                // or the child would have been merged already
                return false;
            }
            totalObjects += child->_objects.size();
        }
    }
    return totalObjects <= NumObjectsAllowed;
}

Ref<PointOctreeBoundingBox> PointOctreeNode::bounds() {
    return _bounds;
}

void PointOctreeNode::getChildBounds(std::vector<Ref<PointOctreeBoundingBox>> bounds) {
    if (hasChildren())
    {
        for (Ref<PointOctreeNode> child : _children)
        {
            child->getChildBounds(bounds);
        }
        return;
    }
    bounds.push_back(_bounds);
}

bool PointOctreeNode::add(Ref<RefCounted> obj, Vector3 objPos) {
    if (!encapsulates(_bounds, objPos))
    {
        return false;
    }
    subAdd(obj, objPos);
    return true;
}

bool PointOctreeNode::remove(Ref<RefCounted> obj) {
    bool removed = false;

    for (int i = 0; i < _objects.size(); i++)
    {
        if (Ref<PointOctreeObject>(_objects[i])->get_obj() == obj)
        {
            removed = true;
            _objects.erase(_objects.begin() + i);
            break;
        }
    }

    if (!removed && _children.size() > 0)
    {
        for (int i = 0; i < 8; i++)
        {

            removed = Ref<PointOctreeNode>(_children[i])->remove(obj);
            if (removed) break;
        }
    }

    if (removed && _children.size() > 0)
    {
        // Check if we should merge nodes now that we've removed an item
        if (shouldMerge())
        {
            merge();
        }
    }

    return removed;
}

bool PointOctreeNode::remove(Ref<RefCounted> obj, Vector3 objPos) {
    if (!encapsulates(_bounds, objPos))
    {
        return false;
    }
    return subRemove(obj, objPos);
}

void PointOctreeNode::getNearby(Ref<PointOctreeRay> ray, float maxDistance, std::vector<Ref<RefCounted>> &result) {
    // Does the ray hit this node at all?
    // Note: Expanding the bounds is not exactly the same as a real distance check, but it's fast.
    // TODO: Does someone have a fast AND accurate formula to do this check?
    _bounds->expand(Vector3(maxDistance * 2, maxDistance * 2, maxDistance * 2));
    bool intersected = _bounds->intersectRay(ray);
    _bounds->set_size(_actualBoundsSize);
    if (!intersected)
    {
        return;
    }

    // Check against any objects in this node
    for (int i = 0; i < _objects.size(); i++)
    {
        if (sqrDistanceToRay(ray, Ref<PointOctreeObject>(_objects[i])->get_pos()) <= (maxDistance * maxDistance))
        {
            result.push_back(Ref<PointOctreeObject>(_objects[i])->get_obj());
        }
    }

    // Check children
    if (_children.size())
    {
        for (int i = 0; i < 8; i++)
        {
            Ref<PointOctreeNode>(_children[i])->getNearby(ray, maxDistance, result);
        }
    }
}

void PointOctreeNode::getNearby(Vector3 &position, float maxDistance, std::vector<Ref<RefCounted>> &result) {
    // Does the node contain this position at all?
    // Note: Expanding the bounds is not exactly the same as a real distance check, but it's fast.
    // TODO: Does someone have a fast AND accurate formula to do this check?
    _bounds->expand(Vector3(maxDistance * 2, maxDistance * 2, maxDistance * 2));
    bool contained = _bounds->contains(position);
    _bounds->set_size(_actualBoundsSize);
    if (!contained)
    {
        return;
    }

    // Check against any objects in this node
    for (int i = 0; i < _objects.size(); i++)
    {
        if (position.distance_to(Ref<PointOctreeObject>(_objects[i])->get_pos()) <= maxDistance)
        {
            result.push_back(Ref<PointOctreeObject>(_objects[i])->get_obj());
        }
    }

    // Check children
    if (_children.size() > 0)
    {
        for (int i = 0; i < 8; i++)
        {
            Ref<PointOctreeNode>(_children[i])->getNearby(position, maxDistance, result);
        }
    }
}

void PointOctreeNode::getAll(std::vector<Ref<RefCounted>> &result) {
    // add directly contained objects
    for (Ref<PointOctreeObject> object : _objects) {
        result.push_back(object->get_obj());
    }

    // add children objects
    if (_children.size() > 0)
    {
        for (int i = 0; i < 8; i++)
        {
            Ref<PointOctreeNode>(_children[i])->getAll(result);
        }
    }
}

void PointOctreeNode::setChildren(std::vector<Ref<PointOctreeNode>> &childOctrees) {
    ERR_FAIL_COND_MSG(childOctrees.size() != 8, "Child octree array must be length 8. Was length: " + String::num_int64(childOctrees.size()));

    _children = childOctrees;
}

Ref<PointOctreeNode> PointOctreeNode::shrinkIfPossible(float minLength) {
    if (_sideLength < (2 * minLength))
    {
        return this;
    }
    if (_objects.size() == 0 && (_children.size() == 0))
    {
        return this;
    }

    // Check objects in root
    int bestFit = -1;
    for (int i = 0; i < _objects.size(); i++)
    {
        Ref<PointOctreeObject> curObj = _objects[i];
        int newBestFit = bestFitChild(curObj->get_pos());
        if (i == 0 || newBestFit == bestFit)
        {
            if (bestFit < 0)
            {
                bestFit = newBestFit;
            }
        }
        else
        {
            return this; // Can't reduce - objects fit in different octants
        }
    }

    // Check objects in children if there are any
    if (_children.size() > 0)
    {
        bool childHadContent = false;
        for (int i = 0; i < _children.size(); i++)
        {
            if (Ref<PointOctreeNode>(_children[i])->hasAnyObjects())
            {
                if (childHadContent)
                {
                    return this; // Can't shrink - another child had content already
                }
                if (bestFit >= 0 && bestFit != i)
                {
                    return this; // Can't reduce - objects in root are in a different octant to objects in child
                }
                childHadContent = true;
                bestFit = i;
            }
        }
    }

    // Can reduce
    if (_children.size() == 0)
    {
        // We don't have any children, so just shrink this node to the new size
        // We already know that everything will still fit in it
        setValues(_sideLength / 2, _minSize, Ref<PointOctreeBoundingBox>(_childBounds[bestFit])->get_center());
        return this;
    }

    // We have children. Use the appropriate child as the new root node
    return _children[bestFit];
}

int PointOctreeNode::bestFitChild(Vector3 objPos) {
    return (objPos.x <= _center.x ? 0 : 1) + (objPos.y >= _center.y ? 0 : 4) + (objPos.z <= _center.z ? 0 : 2);
}

bool PointOctreeNode::hasAnyObjects() {
    if (_objects.size() > 0) return true;

    if (_children.size() > 0)
    {
        for (int i = 0; i < 8; i++)
        {
            if (Ref<PointOctreeNode>(_children[i])->hasAnyObjects()) return true;
        }
    }

    return false;
}

float PointOctreeNode::sqrDistanceToRay(Ref<PointOctreeRay> ray, Vector3 point) {
    return ray->get_direction().cross(point - ray->get_origin()).length_squared();
}