#pragma once

#include "GameLib.h"
#include "VoxelObjectPrototype.h"

VoxelObjectVoxelData BuildVoxelBlock(
    VoxelWorld & voxelWorld, const glm::uvec3 & size, const glm::vec3 & color);