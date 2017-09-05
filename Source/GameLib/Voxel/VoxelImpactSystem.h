#pragma once

#include <vector>

#include <glm/glm.hpp>

class VoxelObject;

class VoxelImpactSystem
{
public:
    std::vector<glm::uvec3> process(
        VoxelObject &      voxelObject,
        const glm::uvec3 & voxel,
        float              intensity,
        float              radius);
};