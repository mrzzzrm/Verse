#include "VoxelClusterSplitSystem.h"

#include <memory>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/RigidBody.h>

#include "Components.h"
#include "VoxelObjectPrototype.h"
#include "VoxelObject.h"
#include "VoxelRigidBodyPayload.h"

VoxelClusterSplitSystem::VoxelClusterSplitSystem(World & world):
    Base(world, ComponentFilter::requires<VoxelObject, RigidBodyComponent>())
{

}

void VoxelClusterSplitSystem::onEntityUpdate(Entity & entity, float seconds)
{
    auto & originalVoxelObject = entity.component<VoxelObject>();
    auto & originalBody = entity.component<RigidBodyComponent>().value();
    auto & originalVoxelData = originalVoxelObject.data();
    const auto & splitDetector = originalVoxelObject.data().splitDetector();
    const auto & splits = splitDetector.splits();

    if (splits.size() <= 1) return;

    for (size_t s = 0; s < splits.size(); s++)
    {
        if (s == splitDetector.splitContainingCrucialVoxel()) continue;

        const auto & split = splits[s];

        std::vector<Voxel> splitVoxels;
        splitVoxels.reserve(split.voxels.size());

        auto color = RandomColorGenerator().generate();

        for (const auto & voxel : split.voxels)
        {
            const auto color = originalVoxelData.voxelColor(voxel);
            const auto healthPoints = originalVoxelData.voxelHealthPoints(voxel);
            Voxel splitVoxel(voxel - split.llf, color, healthPoints);

            splitVoxels.emplace_back(splitVoxel);
        }

        const auto splitSize = split.urb - split.llf + 1u;
        VoxelObjectVoxelData splitVoxelData(originalVoxelObject.data().voxelWorld(), splitSize);
        splitVoxelData.addVoxels(std::move(splitVoxels));

        auto splitEntity = world().createEntity("Split");
        auto & splitVoxelObject = splitEntity.addComponent<VoxelObject>(splitVoxelData);

        auto rigidBodyPayload = std::make_shared<VoxelRigidBodyPayload>(splitVoxelObject.shared_from_this());
        auto splitBody = std::make_shared<RigidBody>(splitVoxelObject.data().shape());
        splitBody->setPayload(rigidBodyPayload);
        splitBody->adjustCenterOfMass();

        const auto relativeCenterOfMass = splitBody->shape()->centerOfMass() + glm::vec3(split.llf) /*+ glm::vec3(0.5f) */-
            originalBody->shape()->centerOfMass();

        const auto splitPosition = originalBody->transform().position() +
            originalBody->transform().orientation() * relativeCenterOfMass;

        splitBody->transform().setPosition(splitPosition);
        splitBody->transform().setOrientation(originalBody->transform().orientation());
        splitBody->setLinearVelocity(
            originalBody->localVelocity(originalBody->transform().pointLocalToWorld(originalBody->transform().center() + relativeCenterOfMass) -
            originalBody->transform().position()));
        splitBody->setAngularVelocity(originalBody->angularVelocity());

        splitEntity.addComponent<RigidBodyComponent>(splitBody);

        originalVoxelObject.removeVoxels(split.voxels);
    }
}