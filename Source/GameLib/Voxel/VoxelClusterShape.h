#pragma once

#include <Deliberation/Physics/CollisionShape.h>

#include "Voxel/VoxelCluster.h"

#include "GameLib.h"

class VoxelClusterShape final:
    public CollisionShape
{
public:
    template<typename T>
    VoxelClusterShape(const VoxelCluster<T> & cluster);

    virtual AABB bounds(const Transform3D & transform) const override;
    virtual glm::mat3 localInertia() const override;
};

#include "Voxel/VoxelClusterShape.inl"