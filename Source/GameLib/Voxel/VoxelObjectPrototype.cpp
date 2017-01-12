#include "VoxelObjectPrototype.h"

#include <Deliberation/Core/ScopeProfiler.h>

#include "VoxelCluster.h"

VoxelObjectPrototype::VoxelObjectPrototype(VoxelWorld & world, const VoxelCluster<glm::vec3> & cluster):
    m_world(world),
    m_cluster(cluster)
{
    m_shape = std::make_shared<VoxelClusterShape>(m_cluster);
    std::cout << m_shape->toString() << std::endl;
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

uint VoxelObjectPrototype::refCount() const
{
    return m_refCount;
}

void VoxelObjectPrototype::incRefCount()
{
    m_refCount++;
}

void VoxelObjectPrototype::decRefCount()
{
    Assert(m_refCount > 0, "Can't dec zero ref count");
    m_refCount--;
}

void VoxelObjectPrototype::removeVoxel(const glm::uvec3 & voxel)
{
    ScopeProfiler scopeProfiler("VoxelObjectPrototype::removeVoxel");

    m_cluster.set(voxel, decltype(m_cluster)::EMPTY_VOXEL);

    m_shape = std::make_shared<VoxelClusterShape>(m_cluster);
    m_model.reset(m_world, m_cluster);
}

std::shared_ptr<VoxelObjectPrototype> VoxelObjectPrototype::clone()
{
    ScopeProfiler scopeProfiler("VoxelObjectPrototype::clone");

    return std::make_shared<VoxelObjectPrototype>(m_world, m_cluster);
}