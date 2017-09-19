#pragma once

#include <Deliberation/ECS/Entity.h>

#include "GameLib.h"
#include "VoxelCluster.h"
#include "VoxelClusterSegment.h"

class VoxelShredder final
{
public:
    std::vector<Entity> shred(const Entity & originalEntity);
    std::vector<VoxelClusterSegment> shred(const VoxelCluster<u32> & input);

private:
    std::vector<VoxelClusterSegment> shred(const std::vector<glm::uvec3> & input,
                                  const glm::uvec3 & llf,
                                  const glm::uvec3 & urb,
                                  u32 depth);

private:
    u32 m_minShredDepth = 1;
};