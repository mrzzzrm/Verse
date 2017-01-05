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
    VoxelCluster(const glm::uvec3 & size);

    const glm::vec3 & size() const;

    void set(const glm::uvec3 & voxel, const T & value);

    T & get(const glm::uvec3 & voxel);
    const T & get(const glm::uvec3 & voxel) const;

    bool test(const glm::uvec3 & voxel) const;

private:
    size_t voxelToIndex(const glm::uvec3 & voxel) const;

private:
    glm::vec3      m_size;
    std::vector<T> m_voxels;
};

#include "VoxelCluster.inl"