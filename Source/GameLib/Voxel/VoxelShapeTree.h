#pragma once

#include <memory>
#include <stdlib.h>
#include <vector>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Sphere.h>

#include "GameLib.h"
#include "Voxel.h"

namespace deliberation
{
class Ray3D;
class Transform3D;
}

class VoxelShapeTree final
{
public:
    VoxelShapeTree(const glm::uvec3 & size);

    void addVoxels(const std::vector<Voxel> & voxels);
    void removeVoxels(const std::vector<glm::uvec3> & voxels);

    bool lineCast(const Transform3D & transform, const Ray3D & ray, glm::uvec3 & voxel) const;

private:
    static constexpr size_t NO_LEAF = std::numeric_limits<size_t>::max();

private:
    template<typename T>
    struct Subtree final
    {
        Subtree(const glm::uvec3 & size, const glm::uvec3 & maxChunkSize);

        void addVoxels(const std::vector<Voxel> & voxels);
        void removeVoxels(const std::vector<glm::uvec3> & voxels);
        void addVoxelToNode(size_t index, const Voxel & voxel);
        void removeVoxelFromNode(size_t index, const glm::uvec3 & voxel);
        void addVoxelToLeaf(size_t index, const Voxel & voxel);
        void removeVoxelFromLeaf(size_t index, const glm::uvec3 & voxel);

        void lineCast(size_t index, const Ray3D & ray, std::vector<glm::uvec3> & voxels) const;
        void lineCastLeaf(size_t index, const Ray3D & ray, std::vector<glm::uvec3> & voxels) const;

        std::shared_ptr<Subtree<T>> clone() const;

        struct Node
        {
            glm::uvec3  llf;
            glm::uvec3  urb;
            Sphere      bounds;
            size_t      leaf = NO_LEAF;
            size_t      numHullVoxels = 0;
        };

        std::vector<Node>   nodes;
        std::vector<T>      leaves;
    };

    struct VoxelLeaf
    {
        bool isSet = false;
        glm::uvec3 cell;
    };

private:
    using ChunkLeaf = std::shared_ptr<Subtree<VoxelLeaf>>;

private:
    glm::uvec3          m_size;
    Subtree<ChunkLeaf>  m_tree;
};
