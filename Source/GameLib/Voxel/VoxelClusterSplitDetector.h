#pragma once

#include <stack>
#include <vector>

#include <Deliberation/Core/Experimental.h>

#include "GameLib.h"
#include "Voxel.h"
#include "VoxelCluster.h"

struct VoxelClusterSplit
{
    std::vector<glm::uvec3> voxels;
    glm::uvec3              llf;
    glm::uvec3              urb;
};

class VoxelClusterSplitDetector final
{
public:
    VoxelClusterSplitDetector(const glm::uvec3 & size);

    const std::vector<VoxelClusterSplit> splits() const;

    size_t splitContainingCrucialVoxel() const;

    void setCrucialVoxel(const glm::uvec3 & crucialVoxel);

    void addVoxels(const std::vector<Voxel> & voxels);
    void removeVoxels(const std::vector<glm::uvec3> & voxels);

private:
    void performSplitDetection();
    void flood(const glm::uvec3 & startVoxel);
    void visit(const glm::ivec3 & voxel);

private:
    u32                             m_currentFloodBeginId = VoxelCluster<u32>::EMPTY_VOXEL + 1;
    u32                             m_currentFloodId = VoxelCluster<u32>::EMPTY_VOXEL + 1;
    VoxelCluster<u32>               m_cluster;
    std::vector<VoxelClusterSplit>  m_splits;
    std::stack<glm::uvec3>          m_floodStack;
    size_t                          m_splitContainingCrucialVoxel = 0;
    std::experimental::optional<glm::uvec3>
                                    m_crucialVoxel;
};


