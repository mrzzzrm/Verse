#pragma once

#include <memory>
#include <vector>

#include <Deliberation/ECS/Entity.h>

#include "Voxel.h"

class VoxelObject;

enum class VoxelRemovalReason
{
    Destruction,
    Split
};

struct VoxelObjectModification
{
    static VoxelObjectModification split(const Entity & entity,
                                           const std::vector<glm::uvec3> & voxels)
    {
        auto modification = VoxelObjectModification(entity);
        modification.splits = voxels;
        return modification;
    }

    static VoxelObjectModification destruction(const Entity & entity,
                                           const std::vector<glm::uvec3> & voxels)
    {
        auto modification = VoxelObjectModification(entity);
        modification.destructions = voxels;
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
        entity(entity)
    {
    }

    Entity                  entity;
    std::vector<Voxel>      additions;
    std::vector<glm::uvec3> destructions;
    std::vector<glm::uvec3> splits;
};