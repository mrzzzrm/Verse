#pragma once

#include <boost/optional.hpp>

#include <glm/glm.hpp>

#include "GameLib.h"
#include "Voxel.h"
#include "VoxelClusterSplitDetector.h"
#include "VoxelHull.h"
#include "VoxelRenderable.h"
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
    VoxelObjectVoxelData(VoxelWorld & voxelWorld,
                         const std::shared_ptr<ColorPalette> & palette,
                         const glm::uvec3 & size);

    VoxelWorld & voxelWorld() const;
    const glm::uvec3 & size() const;
    const VoxelRenderable & renderTree() const;
    const std::shared_ptr<VoxelShape> & shape() const;
    const VoxelHull & hull() const;
    float scale() const;
    size_t numVoxels() const { return m_numVoxels; }
    VoxelClusterSplitDetector & splitDetector() { return m_splitDetector; }
    const VoxelClusterSplitDetector & splitDetector() const { return m_splitDetector; }
    const std::shared_ptr<ColorPalette> & palette() const { return m_renderable.palette(); }

    bool hasVoxel(const glm::ivec3 & voxel) const;

    u32 voxelColorIndex(const glm::uvec3 & voxel) const;
    float voxelHealthPoints(const glm::uvec3 & voxel) const;

    void setVoxelHealthPoints(const glm::uvec3 & voxel, float healthPoints);
    void setScale(float scale);

    void addVoxelsRaw(std::vector<Voxel> voxels);
    void removeVoxelsRaw(std::vector<glm::uvec3> voxels);

    void setCrucialVoxel(const boost::optional<glm::uvec3> & crucialVoxel);

    void operator=(const VoxelObjectVoxelData & prototype) = delete;
    void operator=(VoxelObjectVoxelData && prototype) = delete;

private:
    VoxelWorld &                    m_voxelWorld;
    VoxelCluster<u32>               m_colorIndices;
    VoxelCluster<float>             m_healthPoints;
    VoxelRenderable                 m_renderable;
    std::shared_ptr<VoxelShape>     m_shape;
    VoxelHull                       m_hull;
    float                           m_scale = 1.0f;
    size_t                          m_numVoxels = 0;
    VoxelClusterSplitDetector       m_splitDetector;
};