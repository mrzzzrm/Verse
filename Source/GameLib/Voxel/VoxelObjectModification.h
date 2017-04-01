#pragma once

#include <memory>
#include <vector>

#include "Voxel.h"

class VoxelObject;

struct VoxelObjectModification
{
    static VoxelObjectModification removal(const std::shared_ptr<VoxelObject> & object,
                                           const std::vector<glm::uvec3> & removals)
    {
        auto modification = VoxelObjectModification(object);
        modification.removals = removals;
        return modification;
    }

    static VoxelObjectModification addition(const std::shared_ptr<VoxelObject> & object,
                                            const std::vector<Voxel> & additions)
    {
        auto modification = VoxelObjectModification(object);
        modification.additions = additions;
        return modification;
    }

    VoxelObjectModification(std::shared_ptr<VoxelObject> object):
        object(object)
    {
    }

    std::shared_ptr<VoxelObject>    object;
    std::vector<Voxel>              additions;
    std::vector<glm::uvec3>         removals;
};