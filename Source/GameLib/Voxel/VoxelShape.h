#pragma once

#include <memory>
#include <stdlib.h>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Sphere.h>

#include <Deliberation/Physics/CollisionShape.h>

#include "GameLib.h"
#include "Voxel.h"

namespace deliberation
{
class Ray3D;
class Transform3D;
} // namespace deliberation

class VoxelShape final : public CollisionShape
{
public:
    VoxelShape(const glm::uvec3 & size);

    // From CollisionShape
    AABB      bounds(const Transform3D & transform) const override;
    glm::vec3 localInertia(float scale) const override;
    float     mass(float scale) const override;
    glm::vec3 centerOfMass() const override;

    void updateVoxel(const glm::uvec3 & voxel, bool set);

    bool lineCast(
        const Transform3D & transform,
        const Ray3D &       ray,
        glm::uvec3 &        voxel) const;

    std::string toString() const;

private:
    static constexpr size_t NO_LEAF = std::numeric_limits<size_t>::max();

private:
    template<typename T>
    struct Subtree final
    {
        Subtree(const glm::uvec3 & size, const glm::uvec3 & maxChunkSize);

        void updateVoxel(size_t index, const glm::uvec3 & voxel, bool set);
        void updateVoxelLeaf(size_t index, const glm::uvec3 & voxel, bool set);

        void lineCast(
            size_t                    index,
            const Ray3D &             ray,
            std::vector<glm::uvec3> & voxels) const;
        void lineCastLeaf(
            size_t                    index,
            const Ray3D &             ray,
            std::vector<glm::uvec3> & voxels) const;

        std::shared_ptr<Subtree<T>> clone() const;

        void toString(
            std::stringstream & stream, size_t index, size_t indentation) const;
        void leafToString(
            std::stringstream & stream, size_t index, size_t indentation) const;

        struct Node
        {
            glm::uvec3 llf;
            glm::uvec3 urb;
            Sphere     bounds;
            size_t     leaf = NO_LEAF;
            size_t     numVoxels = 0;
        };

        std::vector<Node> nodes;
        std::vector<T>    leaves;
    };

private:
    using VoxelLeaf = bool;
    using ChunkLeaf = std::shared_ptr<Subtree<VoxelLeaf>>;

private:
    void updateMassProperties() const;

private:
    glm::uvec3         m_size;
    Subtree<ChunkLeaf> m_tree;
    mutable glm::vec3  m_localInertia;
    mutable glm::vec3  m_centerOfMass;
    glm::uvec3         m_voxelPositionAccumulator;
    u32                m_numVoxels = 0;
    mutable bool       m_massPropertiesDirty = true;
    mutable u32        m_absIXX = 0;
    mutable u32        m_absIYY = 0;
    mutable u32        m_absIZZ = 0;
    mutable u32        m_absIXY = 0;
    mutable u32        m_absIXZ = 0;
    mutable u32        m_absIYZ = 0;
};
