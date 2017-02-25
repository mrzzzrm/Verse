#include "VoxelObjectVoxelData.h"

VoxelObjectVoxelData::VoxelObjectVoxelData(const VoxelObjectVoxelData & prototype):
    m_voxelWorld(prototype.m_voxelWorld),
    m_cluster(prototype.m_cluster),
    m_renderTree(prototype.m_renderTree),
    m_shape(std::make_shared<VoxelShape>(*prototype.m_shape)),
    m_hull(prototype.m_hull),
    m_splitDetector(prototype.m_splitDetector)
{

}

VoxelObjectVoxelData::VoxelObjectVoxelData(const VoxelWorld & voxelWorld, const glm::uvec3 & size):
    m_voxelWorld(voxelWorld),
    m_cluster(size),
    m_renderTree(voxelWorld, size),
    m_shape(std::make_shared<VoxelShape>(size)),
    m_hull(size),
    m_splitDetector(size)
{

}

const VoxelWorld & VoxelObjectVoxelData::voxelWorld() const
{
    return m_voxelWorld;
}

const glm::uvec3 & VoxelObjectVoxelData::size() const
{
    return m_cluster.size();
}

const VoxelCluster<Voxel> & VoxelObjectVoxelData::cluster() const
{
    return m_cluster;
}

const VoxelRenderChunkTree & VoxelObjectVoxelData::renderTree() const
{
    return m_renderTree;
}

const std::shared_ptr<VoxelShape>  & VoxelObjectVoxelData::shape() const
{
    return m_shape;
}

const VoxelHull & VoxelObjectVoxelData::hull() const
{
    return m_hull;
}

const VoxelClusterSplitDetector & VoxelObjectVoxelData::splitDetector() const
{
    return m_splitDetector;
}

const Voxel & VoxelObjectVoxelData::voxel(const glm::uvec3 & cell) const
{
    return m_cluster.getRef(cell);
}

void VoxelObjectVoxelData::addVoxels(std::vector<Voxel> voxels)
{
    m_hull.addVoxels(voxels);

    for (auto & voxel : voxels)
    {
        m_cluster.set(voxel.cell, voxel);
        m_renderTree.addVoxel(voxel, m_hull.isHullVoxel(voxel.cell));
    }

    for (auto & voxel : m_hull.newHullVoxels())
    {
        m_shape->updateVoxel(voxel, true);
    }

    for (auto & voxel : m_hull.newObscuredVoxels())
    {
        m_renderTree.updateVoxelVisibility(voxel, false);
        m_shape->updateVoxel(voxel, false);
    }

    m_splitDetector.addVoxels(voxels);
}

void VoxelObjectVoxelData::removeVoxels(const std::vector<glm::uvec3> & voxels)
{
    for (auto & voxel : voxels)
    {
        m_cluster.set(voxel, VoxelCluster<Voxel>::EMPTY_VOXEL);
        if (m_hull.isHullVoxel(voxel)) m_shape->updateVoxel(voxel, false);
        m_renderTree.removeVoxel(voxel, m_hull.isHullVoxel(voxel));
    }

    m_hull.removeVoxels(voxels);

    for (auto & voxel : m_hull.newHullVoxels())
    {
        m_shape->updateVoxel(voxel, true);
        m_renderTree.updateVoxelVisibility(voxel, true);
    }

    m_splitDetector.removeVoxels(voxels);
}
