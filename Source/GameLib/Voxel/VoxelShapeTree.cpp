#include "VoxelShapeTree.h"

#include <functional>

#include <Deliberation/Core/Assert.h>

VoxelShapeTree::VoxelShapeTree(const glm::uvec3 & size):
    m_size(size)
{
    Assert(m_size.x >= 0 && m_size.y >= 0 && m_size.z >= 0, "");

    size_t numLeafs = 0;

    glm::uvec3 maxChunkSize(6); // Must be > 2

    std::function<void(size_t, const glm::ivec3 &, const glm::ivec3 &)> buildTree = [&] (
        size_t index,
        const glm::ivec3 & llf,
        const glm::ivec3 & urb
    )
    {
        if (index >= m_nodes.size()) m_nodes.resize(index + 1);

        auto & node = m_tree.nodes[index];

        node.llf = llf;
        node.urb = urb;

        auto size = urb - llf + 1;

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

            auto separationIndex = (urbRender[longestAxis] + llfRender[longestAxis]) / 2;

            auto urbLeft = urb;
            urbLeft[longestAxis] = separationIndex;

            auto llfRight = llf;
            llfRight[longestAxis] = separationIndex - 1;

            buildTree(index * 2 + 1, llf, urbLeft);
            buildTree(index * 2 + 2, llfRight, urb);
        }
    };

    auto llf = glm::ivec3(0);
    auto urb = glm::ivec3(m_size) - 1;

    buildTree(0, llf, urb);

    m_tree.leafs.resize(numLeafs);
}

void VoxelShapeTree::addVoxels(const std::vector<Voxel> & voxels)
{
    m_tree.addVoxels(voxels);
}

void VoxelShapeTree::removeVoxels(const std::vector<glm::uvec3> & voxels)
{
    m_tree.removeVoxels(voxels);
}

bool VoxelShapeTree::lineCast(const Ray3D & ray, glm::uvec3 & voxel) const
{
    return false;
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
        addVoxelToLeaf(node.leaf, voxel);
    }
}

template<typename T>
void VoxelShapeTree::Subtree<T>::removeVoxelFromNode(size_t index, const Voxel & voxel)
{

}

template<>
void VoxelShapeTree::Subtree<std::shared_ptr<Subtree>>::addVoxelToLeaf(size_t index, const Voxel & voxel)
{
    auto & leaf = leafs[index];

    if (leaf)
    {
        if (leaf.use_count() > 1)
        {
            leaf = leaf->clone();
        }
    }
    else
    {
        leaf = std::make_shared<Subtree<bool>>();
    }

    leaf->addVoxelToNode(0, voxel);
}

template<>
void VoxelShapeTree::Subtree<bool>::addVoxelToLeaf(size_t index, const Voxel & voxel)
{

}
