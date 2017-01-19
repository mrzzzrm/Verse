#include "VoxelObjectVoxelData.h"

VoxelObjectVoxelData::VoxelObjectVoxelData(const VoxelWorld & voxelWorld, const glm::uvec3 & size):
    m_voxelWorld(voxelWorld),
    m_size(size),
    m_renderTree(voxelWorld, m_size)
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

const VoxelShapeChunkTree & VoxelObjectVoxelData::shapeTree() const
{
    return m_shapeTree;
}

void VoxelObjectVoxelData::addVoxels(std::vector<Voxel> voxels)
{
    m_renderTree.addVoxels(voxels);
}

void VoxelObjectVoxelData::removeVoxels(const std::vector<glm::uvec3> & voxels)
{
    m_renderTree.removeVoxels(voxels);
}
