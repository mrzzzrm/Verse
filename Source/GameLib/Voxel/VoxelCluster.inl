#pragma once

#include <Deliberation/Core/Assert.h>

template<typename T>
VoxelCluster<T>::VoxelCluster(const glm::uvec3 & size):
    m_size(size),
    m_voxels(size.x * size.y * size.z, EMPTY_VOXEL)
{
}

template<typename T>
const glm::vec3 & VoxelCluster<T>::size() const
{
    return m_size;
}

template<typename T>
const std::vector<T> & VoxelCluster<T>::voxels() const
{
    return m_voxels;
}

template<typename T>
void VoxelCluster<T>::set(const glm::uvec3 & voxel, const T & value)
{
    auto index = voxelToIndex(voxel);
    m_voxels[index] = value;
}

template<typename T>
T VoxelCluster<T>::get(const glm::uvec3 & voxel)
{
    auto index = voxelToIndex(voxel);
    return m_voxels[index];
}

template<typename T>
const T VoxelCluster<T>::get(const glm::uvec3 & voxel) const
{
    auto index = voxelToIndex(voxel);
    return m_voxels[index];
}

template<typename T>
bool VoxelCluster<T>::test(const glm::uvec3 & voxel) const
{
    return get(voxel) != EMPTY_VOXEL;
}

template<typename T>
size_t VoxelCluster<T>::voxelToIndex(const glm::uvec3 & voxel) const
{
    auto result = voxel.x + voxel.y * m_size.x + voxel.z * m_size.x * m_size.y;
    Assert(result < m_voxels.size(), "Voxel out of bounds");
    return result;
}
