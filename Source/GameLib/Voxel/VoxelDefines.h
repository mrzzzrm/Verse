#pragma once

#include <limits>

#include "GameLib.h"

using VoxelObjectWorldUID = u64;

constexpr VoxelObjectWorldUID INVALID_VOXEL_OBJECT_WORLD_UID =
    std::numeric_limits<VoxelObjectWorldUID>::max();