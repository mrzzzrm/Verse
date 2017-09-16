#pragma once

#include <stack>
#include <vector>

#include <boost/optional.hpp>

#include "GameLib.h"
#include "Voxel.h"
#include "VoxelCluster.h"
#include "VoxelClusterSegment.h"

class VoxelObjectVoxelData;

class VoxelClusterSplitDetector final
{
public:
    VoxelClusterSplitDetector(const glm::uvec3 & size);

    const std::vector<VoxelClusterSegment> & segments() const;

    size_t splitContainingCrucialVoxel() const;

    void setCrucialVoxel(const boost::optional<glm::uvec3> & crucialVoxel);

    void addVoxels(const std::vector<Voxel> & voxels);
    void removeVoxels(const std::vector<glm::uvec3> & voxels);

    void performSplitDetection();

private:
    void flood(const glm::uvec3 & startVoxel);
    void visit(const glm::ivec3 & voxel);

private:
    u32 m_currentFloodBeginId = VoxelCluster<u32>::EMPTY_VOXEL + 1;
    u32 m_currentFloodId = VoxelCluster<u32>::EMPTY_VOXEL + 1;
    VoxelCluster<u32>              m_cluster;
    std::vector<VoxelClusterSegment> m_splits;
    std::stack<glm::uvec3>         m_floodStack;
    size_t                         m_splitContainingCrucialVoxel = 0;
    boost::optional<glm::uvec3>    m_crucialVoxel;
    size_t                         m_numVoxels = 0;
};
