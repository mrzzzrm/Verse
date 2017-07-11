#pragma once

#include <memory>
#include <vector>

#include <Deliberation/ECS/Entity.h>

#include "Voxel.h"

class VoxelObject;

struct VoxelObjectModification
{
    static VoxelObjectModification removal(const Entity & entity,
                                           const std::vector<glm::uvec3> & removals)
    {
        auto modification = VoxelObjectModification(entity);
        modification.removals = removals;
        return modification;
    }

    static VoxelObjectModification addition(const Entity & entity,
                                            const std::vector<Voxel> & additions)
    {
        auto modification = VoxelObjectModification(entity);
        modification.additions = additions;
        return modification;
    }

    VoxelObjectModification(const Entity & entity):
        object(object)
    {
    }

    Entity                          entity;
    std::vector<Voxel>              additions;
    std::vector<glm::uvec3>         removals;
};