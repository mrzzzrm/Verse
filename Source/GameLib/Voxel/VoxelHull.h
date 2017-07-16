#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "GameLib.h"
#include "Voxel.h"
#include "VoxelCluster.h"

class VoxelHull final
{
public:
    explicit VoxelHull(const glm::uvec3 & size);

    const std::vector<glm::uvec3> & newHullVoxels() const;
    const std::vector<glm::uvec3> & newObscuredVoxels() const;

    void addVoxels(const std::vector<Voxel> & voxels);
    void removeVoxels(const std::vector<glm::uvec3> & voxels);

    bool isHullVoxel(const glm::uvec3 & voxel);

    std::string toString() const;

private:
    void incVoxel(i32 index);
    void decVoxel(i32 index);

private:
    VoxelCluster<u8>        m_cluster;
    std::vector<glm::uvec3> m_newHullVoxels;
    std::vector<glm::uvec3> m_newObscuredVoxels;
    std::vector<size_t>     m_voxelCache;
};