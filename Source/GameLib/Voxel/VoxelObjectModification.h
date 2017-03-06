#pragma once

#include <memory>
#include <vector>

#include "Voxel.h"

class VoxelObject;

struct VoxelObjectModification
{
    VoxelObjectModification(std::shared_ptr<VoxelObject> object):
        object(object)
    {
    }

    std::shared_ptr<VoxelObject>    object;
    std::vector<Voxel>              additions;
    std::vector<glm::uvec3>         removals;
};