#include "VoxelObjectVoxelData.h"

VoxelObjectVoxelData::VoxelObjectVoxelData(const VoxelWorld & voxelWorld, const glm::uvec3 & size):
    m_voxelWorld(voxelWorld),
    m_size(size),
    m_renderTree(voxelWorld, m_size),
    m_shapeTree(m_size),
    m_hull(m_size)
{

}

const VoxelWorld & VoxelObjectVoxelData::voxelWorld() const
{
    return m_voxelWorld;
}

const glm::uvec3 & VoxelObjectVoxelData::size() const
{
    return m_size;
}

const VoxelRenderChunkTree & VoxelObjectVoxelData::renderTree() const
{
    return m_renderTree;
}

const VoxelShapeTree & VoxelObjectVoxelData::shapeTree() const
{
    return m_shapeTree;
}

void VoxelObjectVoxelData::addVoxels(std::vector<Voxel> voxels)
{
    m_hull.addVoxels(voxels);

    for (auto & voxel : voxels)
    {
        if (m_hull.isHullVoxel(voxel.cell)) m_shapeTree.updateVoxel(voxel.cell, true);
        m_renderTree.addVoxel(voxel, m_hull.isHullVoxel(voxel.cell));
    }

    for (auto & voxel : m_hull.newHullVoxels())
    {
        // No render tree update, already happened in addVoxel()!
    }

    for (auto & voxel : m_hull.newObscuredVoxels())
    {
        m_renderTree.updateVoxelVisibility(voxel, false);
    }
}

void VoxelObjectVoxelData::removeVoxels(const std::vector<glm::uvec3> & voxels)
{
    for (auto & voxel : voxels)
    {
        if (m_hull.isHullVoxel(voxel)) m_shapeTree.updateVoxel(voxel, false);
        m_renderTree.removeVoxel(voxel, m_hull.isHullVoxel(voxel));
    }

    m_hull.removeVoxels(voxels);

    for (auto & voxel : m_hull.newHullVoxels())
    {
        m_shapeTree.updateVoxel(voxel, true);
        m_renderTree.updateVoxelVisibility(voxel, true);
    }
}
