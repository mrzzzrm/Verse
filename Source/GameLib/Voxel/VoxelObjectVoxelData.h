#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Experimental.h>

#include "GameLib.h"
#include "Voxel.h"
#include "VoxelClusterSplitDetector.h"
#include "VoxelHull.h"
#include "VoxelRenderChunkTree.h"
#include "VoxelShape.h"

class VoxelWorld;
class VoxReader;

class VoxelObjectVoxelData final
{
public:
    static std::shared_ptr<VoxelObjectVoxelData> fromFile(VoxReader & voxReader,
                                                          VoxelWorld & voxelWorld,
                                                          const std::string & path);

public:
    VoxelObjectVoxelData(const VoxelObjectVoxelData & prototype);
    VoxelObjectVoxelData(VoxelWorld & voxelWorld, const glm::uvec3 & size);

    VoxelWorld & voxelWorld() const;
    const glm::uvec3 & size() const;
    const VoxelRenderChunkTree & renderTree() const;
    const std::shared_ptr<VoxelShape> & shape() const;
    const VoxelHull & hull() const;
    float scale() const;
    size_t numVoxels() const { return m_numVoxels; }
    VoxelClusterSplitDetector & splitDetector() { return m_splitDetector; }
    const VoxelClusterSplitDetector & splitDetector() const { return m_splitDetector; }

    bool hasVoxel(const glm::ivec3 & voxel) const;

    const glm::vec3 & voxelColor(const glm::uvec3 & voxel) const;
    float voxelHealthPoints(const glm::uvec3 & voxel) const;

    void setVoxelHealthPoints(const glm::uvec3 & voxel, float healthPoints);
    void setScale(float scale);

    void addVoxels(std::vector<Voxel> voxels);
    void removeVoxels(const std::vector<glm::uvec3> & voxels);

    void setCrucialVoxel(const std::experimental::optional<glm::uvec3> & crucialVoxel);

    void operator=(const VoxelObjectVoxelData & prototype) = delete;
    void operator=(VoxelObjectVoxelData && prototype) = delete;

private:
    VoxelWorld &                    m_voxelWorld;
    VoxelCluster<glm::vec3>         m_colors;
    VoxelCluster<float>             m_healthPoints;
    VoxelRenderChunkTree            m_renderTree;
    std::shared_ptr<VoxelShape>     m_shape;
    VoxelHull                       m_hull;
    float                           m_scale = 1.0f;
    size_t                          m_numVoxels = 0;
    VoxelClusterSplitDetector       m_splitDetector;
};