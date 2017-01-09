#include "VoxelObjectPrototype.h"

VoxelObjectPrototype::VoxelObjectPrototype(VoxelWorld & world, const VoxelCluster<glm::vec3> & cluster):
    m_world(world),
    m_cluster(cluster)
{
    m_shape = std::make_shared<VoxelClusterShape>(m_cluster);
    m_model.reset(world, m_cluster);
}

const VoxelCluster<glm::vec3> & VoxelObjectPrototype::cluster() const
{
    return m_cluster;
}

const std::shared_ptr<VoxelClusterShape> & VoxelObjectPrototype::shape() const
{
    return m_shape;
}

VoxelModel & VoxelObjectPrototype::model()
{
    return *m_model;
}