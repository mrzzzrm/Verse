#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "GameLib.h"

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

    const glm::uvec3 & size() const;
    std::vector<T> & voxels();
    const std::vector<T> & voxels() const;

    void set(const glm::uvec3 & voxel, const T & value);
    void set(size_t index, const T & value);

    T & get(const glm::uvec3 & voxel);
    const T & get(const glm::uvec3 & voxel) const;
    T & get(size_t index);
    const T & get(size_t index) const;

    bool contains(const glm::ivec3 & voxel) const;
    bool contains(i32 index);

    bool test(const glm::uvec3 & voxel) const;
    bool test(size_t index) const;

    size_t voxelToIndex(const glm::uvec3 & voxel) const;
    glm::uvec3 indexToVoxel(size_t index) const;

private:
    glm::uvec3      m_size;
    u32             m_sliceLength = 0;
    std::vector<T>  m_voxels;
};

#include "VoxelCluster.inl"