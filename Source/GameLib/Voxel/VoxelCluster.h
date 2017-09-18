#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "GameLib.h"

// template<typename T> class VoxelCluster;
//
// template<typename T>
// struct VoxelClusterIterator final
//{
//    VoxelClusterIterator(const VoxelCluster<T> & cluster, const glm::uvec3 &
//    voxel);
//
//    VoxelClusterIterator operator++();
//    const T & operator*() const;
//    bool operator!=(const VoxelClusterIterator & rhs) const;
//
// private:
//    const VoxelCluster<T> &     m_cluster;
//    glm::uvec3                  m_voxel;
//};

template<typename T>
class VoxelCluster final
{
public:
    static const T EMPTY_VOXEL;

public:
    VoxelCluster() = default;
    VoxelCluster(const glm::uvec3 & size);

    u32 lineLength() const;
    u32 sliceLength() const;

    const glm::uvec3 &     size() const;
    std::vector<T> &       voxels();
    const std::vector<T> & voxels() const;

    /**
     * @return Number of non-empty voxels
     */
    u32 numVoxels() const { return m_numVoxels; }

    void set(const glm::uvec3 & voxel, const T & value);
    void set(size_t index, const T & value);

    T get(const glm::uvec3 & voxel) const;
    T get(size_t index) const;

    T &       getRef(const glm::uvec3 & voxel);
    const T & getRef(const glm::uvec3 & voxel) const;
    T &       getRef(size_t index);
    const T & getRef(size_t index) const;

    bool contains(const glm::ivec3 & voxel) const;
    bool contains(i32 index);

    bool test(const glm::uvec3 & voxel) const;
    bool test(size_t index) const;

    size_t     voxelToIndex(const glm::uvec3 & voxel) const;
    glm::uvec3 indexToVoxel(size_t index) const;

    //    VoxelClusterIterator begin() const;
    //    VoxelClusterIterator end() const;

private:
    glm::uvec3     m_size;
    u32            m_sliceLength = 0;
    std::vector<T> m_voxels;
    u32            m_numVoxels = 0;
};

#include "VoxelCluster.inl"