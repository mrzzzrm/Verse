#include "VoxelImpactSystem.h"

#include <cmath>
#include <unordered_set>

#include <glm/detail/setup.hpp>
#include <glm/gtx/hash.hpp>

#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/ECS/World.h>

#include "VoxelObject.h"
#include "VoxelObjectPrototype.h"
#include "VoxelObjectModification.h"
#include "VoxelWorld.h"

std::vector<glm::uvec3> VoxelImpactSystem::process(
    VoxelObject & voxelObject,
    const glm::uvec3 & originVoxel,
    float intensity,
    float radius)
{
    auto & voxelWorld = voxelObject.voxelWorld();

    const auto maxDepth = (u32)(std::ceil(radius * 1.5f));
    auto & voxelData = voxelObject.data();

    if (!voxelData->hasVoxel(originVoxel)) return {};

    const auto radius2 = radius * radius;

    std::unordered_set<glm::uvec3> currentDepthSet;
    std::unordered_set<glm::uvec3> nextDepthSet;
    std::vector<glm::uvec3> voxelRemovals;

    currentDepthSet.insert(originVoxel);

    auto visit = [&] (u32 depth, const glm::ivec3 & voxel)
    {
        const auto diffToOrigin = glm::abs(voxel - glm::ivec3(originVoxel));
        const auto manhattan = diffToOrigin.x + diffToOrigin.y + diffToOrigin.z;

        if (!voxelData->hasVoxel(voxel)) return;

        if (glm::length2(glm::vec3(diffToOrigin)) >= radius2) return;

        if (manhattan == depth) nextDepthSet.insert(glm::uvec3(voxel));
    };

    for (u32 d = 0; d < maxDepth; d++)
    {
        while (!currentDepthSet.empty())
        {
            const auto iter = currentDepthSet.cbegin();
            const auto voxel = *iter;
            currentDepthSet.erase(iter);

            const auto currentHealthPoints = voxelData->voxelHealthPoints(voxel);

            if (intensity > currentHealthPoints)
            {
                voxelRemovals.emplace_back(voxel);

                visit(d + 1, glm::ivec3(voxel) + glm::ivec3(1, 0, 0));
                visit(d + 1, glm::ivec3(voxel) + glm::ivec3(0, 1, 0));
                visit(d + 1, glm::ivec3(voxel) + glm::ivec3(0, 0, 1));
                visit(d + 1, glm::ivec3(voxel) - glm::ivec3(1, 0, 0));
                visit(d + 1, glm::ivec3(voxel) - glm::ivec3(0, 1, 0));
                visit(d + 1, glm::ivec3(voxel) - glm::ivec3(0, 0, 1));
            }
            else
            {
                voxelObject.setVoxelHealthPoints(voxel, currentHealthPoints - intensity);
            }
        }

        std::swap(currentDepthSet, nextDepthSet);
    }

    auto entity = voxelObject.voxelWorld().world().entityById(voxelObject.entityId());
    voxelWorld.addVoxelObjectModification(
        VoxelObjectModification::removal(entity, voxelRemovals));

    return voxelRemovals;
}