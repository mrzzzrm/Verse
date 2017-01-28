#include "VoxelObjectVoxelData.h"

VoxelObjectVoxelData::VoxelObjectVoxelData(const VoxelWorld & voxelWorld, const glm::uvec3 & size):
    m_voxelWorld(voxelWorld),
    m_cluster(size),
    m_renderTree(voxelWorld, size),
    m_shapeTree(std::make_shared<VoxelShapeTree>(size)),
    m_hull(size)
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

const VoxelCluster<bool> & VoxelObjectVoxelData::cluster() const
{
    return m_cluster;
}

const VoxelRenderChunkTree & VoxelObjectVoxelData::renderTree() const
{
    return m_renderTree;
}

const std::shared_ptr<VoxelShapeTree>  & VoxelObjectVoxelData::shapeTree() const
{
    return m_shapeTree;
}

const VoxelHull & VoxelObjectVoxelData::hull() const
{
    return m_hull;
}

void VoxelObjectVoxelData::addVoxels(std::vector<Voxel> voxels)
{
    m_hull.addVoxels(voxels);

    for (auto & voxel : voxels)
    {
        m_cluster.set(voxel.cell, true);
        m_renderTree.addVoxel(voxel, m_hull.isHullVoxel(voxel.cell));
    }

    for (auto & voxel : m_hull.newHullVoxels())
    {
        m_shapeTree->updateVoxel(voxel, true);
    }

    for (auto & voxel : m_hull.newObscuredVoxels())
    {
        m_renderTree.updateVoxelVisibility(voxel, false);
        m_shapeTree->updateVoxel(voxel, false);
    }
}

void VoxelObjectVoxelData::removeVoxels(const std::vector<glm::uvec3> & voxels)
{
    for (auto & voxel : voxels)
    {
        m_cluster.set(voxel, false);
        if (m_hull.isHullVoxel(voxel)) m_shapeTree->updateVoxel(voxel, false);
        m_renderTree.removeVoxel(voxel, m_hull.isHullVoxel(voxel));
    }

    m_hull.removeVoxels(voxels);

    for (auto & voxel : m_hull.newHullVoxels())
    {
        m_shapeTree->updateVoxel(voxel, true);
        m_renderTree.updateVoxelVisibility(voxel, true);
    }
}
