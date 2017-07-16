#pragma once

#include <vector>

#include <glm/glm.hpp>

class VoxelObject;

class VoxelImpactSystem
{
public:
    void process(VoxelObject & voxelObject, const glm::uvec3 & voxel, float intensity, float radius);
};