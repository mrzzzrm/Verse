#include "VoxelClusterSplitSystem.h"

#include <memory>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/RigidBody.h>

#include "VoxelObjectVoxelData.h"
#include "VoxelObject.h"
#include "VoxelRigidBodyPayload.h"

VoxelClusterSplitSystem::VoxelClusterSplitSystem(World & world):
    Base(world, ComponentFilter::requires<std::shared_ptr<VoxelObject>, std::shared_ptr<RigidBody>>())
{

}

void VoxelClusterSplitSystem::onUpdate(Entity & entity, float seconds)
{
    auto & originalVoxelObject = entity.component<std::shared_ptr<VoxelObject>>();
    auto & originalBody = entity.component<std::shared_ptr<RigidBody>>();
    auto & originalVoxelData = originalVoxelObject->data();
    const auto & splitDetector = originalVoxelObject->data().splitDetector();
    const auto & splits = splitDetector.splits();

    if (splits.size() <= 1) return;

    for (size_t s = 1; s < splits.size(); s++)
    {
        const auto & split = splits[s];

        std::vector<Voxel> splitVoxels;
        splitVoxels.reserve(split.voxels.size());

        auto color = RandomColorGenerator().generate();

        for (const auto & voxel : split.voxels)
        {
            auto splitVoxel = originalVoxelData.voxel(voxel);
            splitVoxel.cell -= split.llf;
            splitVoxel.color = color;

            splitVoxels.emplace_back(splitVoxel);
        }

        const auto splitSize = split.urb - split.llf + 1u;
        VoxelObjectVoxelData splitVoxelData(originalVoxelObject->data().voxelWorld(), splitSize);
        splitVoxelData.addVoxels(std::move(splitVoxels));

        auto splitVoxelObject = std::make_shared<VoxelObject>(splitVoxelData);

        auto rigidBodyPayload = std::make_shared<VoxelRigidBodyPayload>(splitVoxelObject);
        auto splitBody = std::make_shared<RigidBody>(splitVoxelObject->data().shape());
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

        auto splitEntity = world().createEntity("Split");
        splitEntity.addComponent<std::shared_ptr<VoxelObject>>(splitVoxelObject);
        splitEntity.addComponent<std::shared_ptr<RigidBody>>(splitBody);

        originalVoxelObject->removeVoxels(split.voxels);
    }
}