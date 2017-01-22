#include "VoxelShapeTree.h"

#include <functional>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Core/Math/PrimitiveIntersection.h>
#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/Math/Ray3D.h>

VoxelShapeTree::VoxelShapeTree(const glm::uvec3 & size):
    m_size(size),
    m_tree(m_size, glm::uvec3(1))
{

}

void VoxelShapeTree::addVoxels(const std::vector<Voxel> & voxels)
{
    m_tree.addVoxels(voxels);
}

void VoxelShapeTree::removeVoxels(const std::vector<glm::uvec3> & voxels)
{
    m_tree.removeVoxels(voxels);
}

void VoxelShapeTree::updateHull(const glm::uvec3 & voxel, bool hull)
{
    m_tree.updateHull(0, voxel, hull);
}

bool VoxelShapeTree::lineCast(const Transform3D & transform, const Ray3D & ray, glm::uvec3 & voxel) const
{
    std::vector<glm::uvec3> voxels;

    auto localOrigin = transform.pointWorldToLocal(ray.origin());
    auto localDirection = transform.directionWorldToLocal(ray.direction());

    m_tree.lineCast(0, Ray3D(localOrigin, localDirection), voxels);

    if (voxels.empty()) return false;

    auto minimumSquaredDistance = std::numeric_limits<float>::max();
    auto minimumDistanceVoxel = size_t(0);

    for (size_t v = 0; v < voxels.size(); v++)
    {
        auto position = glm::vec3(voxels[v]) + glm::vec3(0.5f);
        auto delta = position - ray.origin();
        auto squaredDistance = glm::dot(delta, delta);

        if (squaredDistance < minimumSquaredDistance)
        {
            minimumSquaredDistance = squaredDistance;
            minimumDistanceVoxel = v;
        }
    }

    voxel = voxels[minimumDistanceVoxel];

    return true;
}

template<typename T>
VoxelShapeTree::Subtree<T>::Subtree(const glm::uvec3 & size, const glm::uvec3 & maxChunkSize)
{
    Assert(size.x >= 0 && size.y >= 0 && size.z >= 0, "");

    size_t numLeafs = 0;

    std::function<void(size_t, const glm::ivec3 &, const glm::ivec3 &)> buildTree = [&] (
        size_t index,
        const glm::ivec3 & llf,
        const glm::ivec3 & urb
    )
    {
        if (index >= nodes.size()) nodes.resize(index + 1);

        auto & node = nodes[index];

        node.llf = llf;
        node.urb = urb;

        auto size = urb - llf + 1;

        auto position = glm::vec3(llf) + glm::vec3(size) / 2.0f;
        auto radius = glm::length(glm::vec3(size) / 2.0f);

        node.bounds = Sphere(position, radius);

        std::cout << index << " " << llf << urb << " " << node.bounds << std::endl;

        if (size.x <= maxChunkSize.x &&
            size.y <= maxChunkSize.y &&
            size.z <= maxChunkSize.z)
        {
            node.leaf = numLeafs;
            numLeafs++;
        }
        else
        {
            auto longestAxis = 0;

            if (size.x >= size.y && size.x >= size.z) longestAxis = 0;
            else if (size.y >= size.x && size.y >= size.z) longestAxis = 1;
            else longestAxis = 2;

            auto separationIndex = (urb[longestAxis] + llf[longestAxis]) / 2;

            auto urbLeft = urb;
            urbLeft[longestAxis] = separationIndex;

            auto llfRight = llf;
            llfRight[longestAxis] = separationIndex + 1;

            buildTree(index * 2 + 1, llf, urbLeft);
            buildTree(index * 2 + 2, llfRight, urb);
        }
    };

    auto llf = glm::ivec3(0);
    auto urb = glm::ivec3(size) - 1;

    buildTree(0, llf, urb);

    leaves.resize(numLeafs);
}

template<typename T>
void VoxelShapeTree::Subtree<T>::updateHull(size_t index, const glm::uvec3 & voxel, bool hull)
{
    auto & node = nodes[index];

    if (hull) node.numHullVoxels++;
    else node.numHullVoxels--;

    if (node.leaf != NO_LEAF)
    {
        updateHullLeaf(index, voxel, hull);
    }
    else
    {
        updateHull(index * 2 + 1, voxel, hull);
        updateHull(index * 2 + 2, voxel, hull);
    }
}

template<>
void VoxelShapeTree::Subtree<VoxelShapeTree::ChunkLeaf>::updateHullLeaf(size_t index, const glm::uvec3 & voxel, bool hull)
{
    auto & node = nodes[index];
    leaves[node.leaf]->updateHull(0, voxel, hull);
}

template<>
void VoxelShapeTree::Subtree<VoxelShapeTree::VoxelLeaf>::updateHullLeaf(size_t index, const glm::uvec3 & voxel, bool hull)
{
}

template<typename T>
void VoxelShapeTree::Subtree<T>::addVoxels(const std::vector<Voxel> & voxels)
{
    for (auto & voxel : voxels)
    {
        addVoxelToNode(0, voxel);
    }
}

template<typename T>
void VoxelShapeTree::Subtree<T>::removeVoxels(const std::vector<glm::uvec3> & voxels)
{
    for (auto & voxel : voxels)
    {
        removeVoxelFromNode(0, voxel);
    }
}

template<typename T>
void VoxelShapeTree::Subtree<T>::addVoxelToNode(size_t index, const Voxel & voxel)
{
    auto & node = nodes[index];

    if (voxel.hull) node.numHullVoxels++;

    if (node.leaf == NO_LEAF)
    {
        auto & c = voxel.cell;
        auto leftIndex = index * 2 + 1;
        auto & left = nodes[leftIndex];

        if (c.x >= left.llf.x && c.x <= left.urb.x &&
            c.y >= left.llf.y && c.y <= left.urb.y &&
            c.z >= left.llf.z && c.z <= left.urb.z)
        {
            addVoxelToNode(leftIndex, voxel);
        }
        else
        {
            addVoxelToNode(index * 2 + 2, voxel);
        }
    }
    else
    {
        addVoxelToLeaf(index, voxel);
    }
}

template<typename T>
void VoxelShapeTree::Subtree<T>::removeVoxelFromNode(size_t index, const glm::uvec3 & voxel)
{
    auto & node = nodes[index];

    //if (voxel.hull) node.numHullVoxels++;

    if (node.leaf == NO_LEAF)
    {
        auto & c = voxel;
        auto leftIndex = index * 2 + 1;
        auto & left = nodes[leftIndex];

        if (c.x >= left.llf.x && c.x <= left.urb.x &&
            c.y >= left.llf.y && c.y <= left.urb.y &&
            c.z >= left.llf.z && c.z <= left.urb.z)
        {
            removeVoxelFromNode(leftIndex, voxel);
        }
        else
        {
            removeVoxelFromNode(index * 2 + 2, voxel);
        }
    }
    else
    {
        removeVoxelFromLeaf(index, voxel);
    }
}

template<>
void VoxelShapeTree::Subtree<VoxelShapeTree::ChunkLeaf>::addVoxelToLeaf(size_t index, const Voxel & voxel)
{
    auto & node = nodes[index];
    auto & leaf = leaves[node.leaf];

    if (leaf)
    {
        if (leaf.use_count() > 1)
        {
            leaf = leaf->clone();
        }
    }
    else
    {
        auto maxChunkSize = glm::uvec3(1);
        leaf = std::make_shared<Subtree<VoxelLeaf>>(node.urb - node.llf + glm::uvec3(1), maxChunkSize);
    }

    leaf->addVoxelToNode(0, voxel);
}

template<>
void VoxelShapeTree::Subtree<VoxelShapeTree::VoxelLeaf>::addVoxelToLeaf(size_t index, const Voxel & voxel)
{
    auto & node = nodes[index];
    leaves[node.leaf].isSet = true;
    leaves[node.leaf].cell = voxel.cell;
}

template<>
void VoxelShapeTree::Subtree<VoxelShapeTree::ChunkLeaf>::removeVoxelFromLeaf(size_t index, const glm::uvec3 & voxel)
{
    auto & node = nodes[index];
    auto & leaf = leaves[node.leaf];
    Assert(!!leaf, "Voxel doesn't exist");
    leaf->removeVoxelFromNode(0, voxel);
}

template<>
void VoxelShapeTree::Subtree<VoxelShapeTree::VoxelLeaf>::removeVoxelFromLeaf(size_t index, const glm::uvec3 & voxel)
{
    auto & node = nodes[index];
    leaves[node.leaf].isSet = false;
}

template<typename T>
void VoxelShapeTree::Subtree<T>::lineCast(size_t index, const Ray3D & ray, std::vector<glm::uvec3> & voxels) const
{
    auto & node = nodes[index];

    if (node.leaf != NO_LEAF)
    {
        lineCastLeaf(index, ray, voxels);
    }
    else
    {
        auto position = node.bounds.position();
        auto radius = node.bounds.radius();

        if (!LineSphereIntersection(ray.origin(), ray.direction(), position, radius))
        {
            return;
        }

        auto leftIndex = index * 2 + 1;
        auto rightIndex = leftIndex + 1;

        if (nodes[leftIndex].numHullVoxels > 0)
        {
            lineCast(leftIndex, ray, voxels);
        }
        if (nodes[rightIndex].numHullVoxels > 0)
        {
            lineCast(rightIndex, ray, voxels);
        }
    }
}

template<>
void VoxelShapeTree::Subtree<VoxelShapeTree::ChunkLeaf>::lineCastLeaf(size_t index,
                                                                      const Ray3D & ray,
                                                                      std::vector<glm::uvec3> & voxels) const
{
    leaves[nodes[index].leaf]->lineCast(0, ray, voxels);
}

template<>
void VoxelShapeTree::Subtree<VoxelShapeTree::VoxelLeaf>::lineCastLeaf(size_t index,
                                                                  const Ray3D & ray,
                                                                  std::vector<glm::uvec3> & voxels) const
{
    auto & node = nodes[index];
    auto & leaf = leaves[node.leaf];

    if (!leaf.isSet) return;

    if (LineSphereIntersection(ray.origin(),
                               ray.direction(),
                               glm::vec3(leaf.cell) + glm::vec3(0.5f),
                               0.8f))
    {
        voxels.emplace_back(leaf.cell);
    }
}

template<typename T>
std::shared_ptr<VoxelShapeTree::Subtree<T>> VoxelShapeTree::Subtree<T>::clone() const
{

}