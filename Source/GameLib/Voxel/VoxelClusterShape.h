#pragma once

#include <vector>

#include <Deliberation/Core/Math/Ray3D.h>
#include <Deliberation/Core/Math/Sphere.h>

#include <Deliberation/Physics/CollisionShape.h>

#include "VoxelCluster.h"

#include "GameLib.h"

class VoxelClusterShape final:
    public CollisionShape
{
public:
    VoxelClusterShape();

    template<typename T>
    VoxelClusterShape(const VoxelCluster<T> & cluster);

    const glm::uvec3 & size() const;

    virtual AABB bounds(const Transform3D & transform) const override;
    virtual glm::mat3 localInertia() const override;

    bool lineCast(const Transform3D & transform, const Ray3D & ray, glm::uvec3 & voxel) const;

    std::string toString() const;

private:
    friend class VoxelClusterShapeTest_Basic_Test;

private:
    struct Node
    {
        bool isLeaf = false;

        struct
        {
            Sphere bounds;
        } node;

        struct
        {
            glm::uvec3 voxel;
        } leaf;
    };

private:
    void addVoxel(const glm::uvec3 & voxel);
    void addVoxelToNode(u32 index, const glm::uvec3 & voxel, const glm::uvec3 & nodeLLF, const glm::uvec3 & nodeSize);
    void lineCastTree(u32 index, const Ray3D & ray, std::vector<glm::uvec3> & voxels) const;

private:
    glm::uvec3          m_size;
    std::vector<bool>   m_nodeMask;
    std::vector<Node>   m_nodes;
};

#include "Voxel/VoxelClusterShape.inl"