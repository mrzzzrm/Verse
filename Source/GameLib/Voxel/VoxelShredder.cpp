#include "VoxelShredder.h"

#include <Deliberation/Core/Math/Random.h>
#include <Deliberation/ECS/Transform3DComponent.h>
#include <Deliberation/ECS/RigidBodyComponent.h>

#include <glm/glm.hpp>

#include "VoxelClusterSplitSystem.h"

#include "VoxelObject.h"

std::vector<Entity> VoxelShredder::explode(Entity & originalEntity)
{
    std::vector<Entity> segmentEntities;

    auto origin = originalEntity.component<Transform3DComponent>().transform().position();

    VoxelShredder shredder;
    auto entities = shredder.shred(originalEntity);

    for (auto & entity : entities)
    {
        auto & object = entity.component<VoxelObject>();
        auto & originalVoxelData = object.data();
        auto & splitDetector = object.data()->splitDetector();

        object.performSplitDetection();

        const auto & segments = splitDetector.segments();

        AssertM(
            !segments.empty() || originalVoxelData->numVoxels() == 0,
            entity.name() +
            ": there has to be one split, the object itself, by "
                "definition. "
                "Remaining voxels: " +
            std::to_string(originalVoxelData->numVoxels()));

        if (segments.size() <= 1)
        {
            segmentEntities.emplace_back(entity);
            continue;
        }

        for (size_t s = 0; s < segments.size(); s++)
        {
            auto entity2 = VoxelClusterSplitSystem::splitVoxelsOffEntity(entity, segments[s]);
            segmentEntities.emplace_back(entity2);
        }

        entity.scheduleRemoval();
    }

    // Apply explosive forces
    for (auto & entity : segmentEntities) {
        auto & body = entity.component<RigidBodyComponent>().value();
        body->adaptShape();

        const auto direction = GLMSafeNormalize(body->transform().position() - origin);
        const auto intensity = RandomFloat(750.0f, 1800.0f) * body->mass() / 4.0f;

        body->applyCentralImpulse(direction * intensity);
        body->applyTorqueImpulse(0.008f * RandomVec3(1.0f, 2.0f) * intensity);
    }

    originalEntity.scheduleRemoval();

    return segmentEntities;
}

std::vector<Entity> VoxelShredder::shred(const Entity & originalEntity)
{
    auto segments = shred(originalEntity.component<VoxelObject>().data()->colorIndices());

    std::vector<Entity> result;
    result.reserve(segments.size());

    for (auto & segment : segments)
    {
        auto entity = VoxelClusterSplitSystem::splitVoxelsOffEntity(originalEntity, segment);
        result.emplace_back(entity);
    }

    return result;
}

std::vector<VoxelClusterSegment> VoxelShredder::shred(const VoxelCluster<u32> & input)
{
    std::vector<glm::uvec3> voxels;
    voxels.reserve(input.numVoxels());

    for (size_t v = 0; v < input.voxels().size(); ++v)
    {
        if (input.test(v)) voxels.emplace_back(input.indexToVoxel(v));
    }

    return shred(voxels, {0, 0, 0}, input.size(), 0);
}

std::vector<VoxelClusterSegment> VoxelShredder::shred(const std::vector<glm::uvec3> & input,
                                                      const glm::uvec3 & llf,
                                                      const glm::uvec3 & urb,
                                                      u32 depth
)
{
    Log->info("Shredding {} to {}", llf, urb);

    const auto inputCenter = (glm::vec3(llf) + glm::vec3(urb)) / 2.0f;

    const auto separationPlaneNormal = RandomUnitVec3();
    const auto separationPlaneD = glm::dot(separationPlaneNormal, inputCenter);

    std::vector<glm::uvec3> left;
    std::vector<glm::uvec3> right;

    auto leftLlf = glm::uvec3(std::numeric_limits<u32>::max());
    auto leftUrb = glm::uvec3(0);
    auto rightLlf = leftLlf;
    auto rightUrb = leftUrb;

    for (const auto & cell : input) {
        const auto position = glm::vec3(cell);

        if (glm::dot(position, separationPlaneNormal) > separationPlaneD) {
            leftLlf = glm::min(cell, leftLlf);
            leftUrb = glm::max(cell, leftUrb);
            left.emplace_back(cell);
        }
        else {
            rightLlf = glm::min(cell, rightLlf);
            rightUrb = glm::max(cell, rightUrb);
            right.emplace_back(cell);
        }
    }

    if (depth < m_maxShredDepth)
    {
        auto leftShredded = shred(left, leftLlf, leftUrb, depth + 1);
        auto rightShredded = shred(right, rightLlf, rightUrb, depth + 1);

        std::vector<VoxelClusterSegment> result;
        result.resize(leftShredded.size() + rightShredded.size());
        std::copy(leftShredded.begin(), leftShredded.end(), result.begin());
        std::copy(rightShredded.begin(), rightShredded.end(), result.begin() + leftShredded.size());

        return result;
    }
    else
    {
        std::vector<VoxelClusterSegment> result;

        if (!left.empty()) result.emplace_back(VoxelClusterSegment{left, leftLlf, leftUrb});
        if (!right.empty()) result.emplace_back(VoxelClusterSegment{right, rightLlf, rightUrb});

        return result;
    }

}