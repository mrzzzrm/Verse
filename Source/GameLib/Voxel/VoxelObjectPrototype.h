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

private:
    VoxelWorld &                        m_world;
    VoxelCluster<glm::vec3>             m_cluster;
    std::shared_ptr<VoxelClusterShape>  m_shape;
    Optional<VoxelModel>                m_model;
};