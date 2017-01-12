#pragma once

#include <Deliberation/Core/Optional.h>

#include "VoxelCluster.h"
#include "VoxelClusterShape.h"
#include "VoxelModel.h"

class VoxelWorld;

class VoxelObjectPrototype
{
public:
    VoxelObjectPrototype(VoxelWorld & world, const VoxelCluster<glm::vec3> & cluster);

    const VoxelCluster<glm::vec3> & cluster() const;
    const std::shared_ptr<VoxelClusterShape> & shape() const;
    VoxelModel & model();
    uint refCount() const;

    void incRefCount();
    void decRefCount();

    void removeVoxel(const glm::uvec3 & voxel);

    std::shared_ptr<VoxelObjectPrototype> clone();

private:
    VoxelWorld &                        m_world;
    VoxelCluster<glm::vec3>             m_cluster;
    std::shared_ptr<VoxelClusterShape>  m_shape;
    Optional<VoxelModel>                m_model;

    // Number of VoxelObjects using this Prototype
    uint                                m_refCount = 0;
};