#pragma once

#include <iostream>

#include <Deliberation/Core/Math/Morton.h>

#include "CollisionShapeTypes.h"
#include "VoxelCluster.h"
#include "VoxelClusterShape.h"

template<typename T>
VoxelClusterShape::VoxelClusterShape(const VoxelCluster<T> & cluster):
    CollisionShape((int)::CollisionShapeType::VoxelCluster),
    m_size(cluster.size())
{
    auto numCells = cluster.size().x * cluster.size().y * cluster.size().z;

    for (uint z = 0; z < cluster.size().z; z++)
    {
        for (uint y = 0; y < cluster.size().y; y++)
        {
            for (uint x = 0; x < cluster.size().x; x++)
            {
                if (cluster.test({x, y, z}))
                {
                    addVoxel({x, y, z});
                }
            }
        }
    }
}

inline const glm::uvec3 & VoxelClusterShape::size() const
{
    return m_size;
}

inline void VoxelClusterShape::addVoxel(const glm::uvec3 & voxel)
{
    addVoxelToNode(0, voxel, {0, 0, 0}, m_size);
}

inline void VoxelClusterShape::addVoxelToNode(u32 index,
                                              const glm::uvec3 & voxel,
                                              const glm::uvec3 & nodeLLF,
                                              const glm::uvec3 & nodeSize)
{
    if (index >= m_nodeMask.size())
    {
        m_nodeMask.resize(index + 1);
        m_nodes.resize(index + 1);
    }

    if (nodeSize == glm::uvec3(1, 1, 1))
    {
        m_nodeMask[index] = true;
        m_nodes[index].isLeaf = true;
        m_nodes[index].leaf.voxel = voxel;
        return;
    }

    if (!m_nodeMask[index])
    {
        m_nodeMask[index] = true;

        auto position = glm::vec3(nodeLLF) + glm::vec3(nodeSize) / 2.0f;
        auto radius = glm::length(glm::vec3(nodeSize) / 2.0f);

        m_nodes[index].node.bounds = Sphere(position, radius);
    }

    u8 longestAxis = 0;

    if (nodeSize.x >= nodeSize.y && nodeSize.x >= nodeSize.z) longestAxis = 0;
    else if (nodeSize.y >= nodeSize.x && nodeSize.y >= nodeSize.z) longestAxis = 1;
    else longestAxis = 2;

    if (!m_nodeMask[index])
    {
        auto position = glm::vec3(nodeLLF) + glm::vec3(nodeSize) / 2.0f;
        auto radius = glm::length(glm::vec3(nodeSize) / 2.0f);

        m_nodes[index].node.bounds = Sphere(position, radius);
    }


    auto voxelInChild = [&](const glm::uvec3 & childLLF, const glm::uvec3 & childSize)
    {
        auto diff = glm::ivec3(voxel) - glm::ivec3(childLLF);
        return diff.x >= 0 && diff.x < childSize.x &&
            diff.y >= 0 && diff.y < childSize.y &&
            diff.z >= 0 && diff.z < childSize.z;
    };

    auto leftSize = nodeSize;
    leftSize[longestAxis] = std::ceil(leftSize[longestAxis] / 2.0f);

    auto rightSize = nodeSize;
    rightSize[longestAxis] = std::floor(rightSize[longestAxis] / 2.0f);

    if (voxelInChild(nodeLLF, leftSize))
    {
        addVoxelToNode(index * 2 + 1, voxel, nodeLLF, leftSize);
    }
    else
    {
        glm::uvec3 offsetLLF;
        offsetLLF[longestAxis] = leftSize[longestAxis];

        addVoxelToNode(index * 2 + 2, voxel, nodeLLF + offsetLLF, rightSize);
    }
}

