#pragma once

#include "GameLib.h"
#include "VoxelCluster.h"

class VoxelShredder final
{
public:
    VoxelShredder(const VoxelCluster<u32> & inputCluster);

    std::vector<VoxelCluster<u32>> process();

private:
    std::vector<std::vector<glm::uvec3>> shred(const std::vector<glm::uvec3> & input,
                                  const glm::uvec3 & llf,
                                  const glm::uvec3 & urb,
                                u32 depth);

private:
    const VoxelCluster<u32> & m_inputCluster;

    u32 m_minShredDepth = 2;
};