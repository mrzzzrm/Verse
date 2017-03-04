#pragma once

#include <vector>

#include "Voxel.h"

struct VoxelObjectModification
{
    std::vector<Voxel>      additions;
    std::vector<glm::uvec3> removals;
};