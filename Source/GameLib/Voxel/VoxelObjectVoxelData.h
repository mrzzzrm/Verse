#pragma once

#include <experimental/optional>

#include <glm/glm.hpp>

#include "GameLib.h"
#include "Voxel.h"
#include "VoxelClusterSplitDetector.h"
#include "VoxelHull.h"
#include "VoxelRenderChunkTree.h"
#include "VoxelShape.h"

class VoxelWorld;

class VoxelObjectVoxelData final
{
public:
    VoxelObjectVoxelData(const VoxelObjectVoxelData & prototype);
    VoxelObjectVoxelData(const VoxelWorld & voxelWorld, const glm::uvec3 & size);

    const VoxelWorld & voxelWorld() const;
    const glm::uvec3 & size() const;
    const VoxelRenderChunkTree & renderTree() const;
    const std::shared_ptr<VoxelShape> & shape() const;
    const VoxelHull & hull() const;
    const VoxelClusterSplitDetector & splitDetector() const;

    void setCrucialVoxel(const glm::uvec3 & voxel);

    bool hasVoxel(const glm::ivec3 & voxel) const;

    const glm::vec3 & voxelColor(const glm::uvec3 & voxel) const;
    float voxelHealthPoints(const glm::uvec3 & voxel) const;

    void setVoxelHealthPoints(const glm::uvec3 & voxel, float healthPoints);

    void addVoxels(std::vector<Voxel> voxels);
    void removeVoxels(const std::vector<glm::uvec3> & voxels);

    void operator=(const VoxelObjectVoxelData & prototype) = delete;
    void operator=(VoxelObjectVoxelData && prototype) = delete;

private:
    const VoxelWorld &              m_voxelWorld;
    VoxelCluster<glm::vec3>         m_colors;
    VoxelCluster<float>             m_healthPoints;
    VoxelRenderChunkTree            m_renderTree;
    std::shared_ptr<VoxelShape>     m_shape;
    VoxelHull                       m_hull;
    VoxelClusterSplitDetector       m_splitDetector;
};