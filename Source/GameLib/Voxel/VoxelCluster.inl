#pragma once

#include <Deliberation/Core/Assert.h>

// template<typename T>
// VoxelClusterIterator<T>::VoxelClusterIterator(const VoxelCluster<T> &
// cluster, const glm::uvec3 & voxel):
//    m_cluster(cluster),
//    m_voxel(voxel)
//{}

// template<typename T>
// VoxelClusterIterator VoxelClusterIterator<T>::operator++()
//{
//    auto voxel = m_voxel;

//    voxel.x++;
//    if (voxel.x >= m_cluster.size().x)
//    {
//        voxel.x = 0;
//        voxel.y++;
//        if (voxel.y >= m_cluster.size().y)
//        {
//            voxel.y = 0;
//            voxel.z++;
//        }
//    }

//    return {m_cluster, voxel};
//}

// template<typename T>
// const T & VoxelClusterIterator<T>::operator*() const
//{
//    return m_cluster.get(m_voxel);
//}

// template<typename T>
// bool VoxelClusterIterator<T>::operator!=(const VoxelClusterIterator & rhs)
// const
//{
//    Assert(&m_cluster == &rhs.m_cluster, "");
//    return m_voxel != rhs.m_voxel;
//}

template<typename T>
VoxelCluster<T>::VoxelCluster(const glm::uvec3 & size)
    : m_size(size), m_voxels(size.x * size.y * size.z, EMPTY_VOXEL)
{
    m_sliceLength = m_size.x * m_size.y;
}

template<typename T>
u32 VoxelCluster<T>::lineLength() const
{
    return m_size.x;
}

template<typename T>
u32 VoxelCluster<T>::sliceLength() const
{
    return m_sliceLength;
}

template<typename T>
const glm::uvec3 & VoxelCluster<T>::size() const
{
    return m_size;
}

template<typename T>
std::vector<T> & VoxelCluster<T>::voxels()
{
    return m_voxels;
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
void VoxelCluster<T>::set(size_t index, const T & value)
{
    Assert(index < m_voxels.size(), "Voxel out of bounds");
    m_voxels[index] = value;
}

template<typename T>
T VoxelCluster<T>::get(const glm::uvec3 & voxel) const
{
    auto index = voxelToIndex(voxel);
    return m_voxels[index];
}

template<typename T>
T VoxelCluster<T>::get(size_t index) const
{
    Assert(index < m_voxels.size(), "Voxel out of bounds");
    return m_voxels[index];
}

template<typename T>
T & VoxelCluster<T>::getRef(size_t index)
{
    Assert(index < m_voxels.size(), "Voxel out of bounds");
    return m_voxels[index];
}

template<typename T>
const T & VoxelCluster<T>::getRef(size_t index) const
{
    Assert(index < m_voxels.size(), "Voxel out of bounds");
    return m_voxels[index];
}

template<typename T>
T & VoxelCluster<T>::getRef(const glm::uvec3 & voxel)
{
    auto index = voxelToIndex(voxel);
    return m_voxels[index];
}

template<typename T>
const T & VoxelCluster<T>::getRef(const glm::uvec3 & voxel) const
{
    auto index = voxelToIndex(voxel);
    return m_voxels[index];
}

template<typename T>
bool VoxelCluster<T>::contains(const glm::ivec3 & voxel) const
{
    return voxel.x >= 0 && voxel.x < m_size.x && voxel.y >= 0 &&
           voxel.y < m_size.y && voxel.z >= 0 && voxel.z < m_size.z;
}

template<typename T>
bool VoxelCluster<T>::contains(i32 index)
{
    return index >= 0 && index < m_voxels.size();
}

template<typename T>
bool VoxelCluster<T>::test(const glm::uvec3 & voxel) const
{
    return get(voxel) != EMPTY_VOXEL;
}

template<typename T>
bool VoxelCluster<T>::test(size_t index) const
{
    return get(index) != EMPTY_VOXEL;
}

template<typename T>
size_t VoxelCluster<T>::voxelToIndex(const glm::uvec3 & voxel) const
{
    auto result = voxel.x + voxel.y * m_size.x + voxel.z * m_size.x * m_size.y;
    Assert(result < m_voxels.size(), "Voxel out of bounds");
    return result;
}

template<typename T>
glm::uvec3 VoxelCluster<T>::indexToVoxel(size_t index) const
{
    auto x = index % m_size.x;
    auto y = (index / m_size.x) % m_size.y;
    auto z = index / m_sliceLength;

    return {x, y, z};
}

// template<typename T>
// VoxelClusterIterator VoxelCluster<T>::begin() const
//{
//    return {*this, {}};
//}
//
// template<typename T>
// VoxelClusterIterator VoxelCluster<T>::end() const
//{
//    return {*this, {m_size.x, m_size.y - 1, m_size.z - 1}};
//}