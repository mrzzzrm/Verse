#include "VoxelClusterSplitSystem.h"

#include <memory>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/Phase.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/RigidBody.h>

#include "ColorPalette.h"
#include "Components.h"
#include "VoxelObject.h"
#include "VoxelObjectModification.h"
#include "VoxelObjectPrototype.h"
#include "VoxelRigidBodyPayload.h"
#include "VoxelWorld.h"

VoxelClusterSplitSystem::VoxelClusterSplitSystem(World & world)
    : Base(world, ComponentFilter::requires<VoxelObject, RigidBodyComponent>())
{
    activatePhases<GameUpdatePhase>();
}

void VoxelClusterSplitSystem::onEntityGameUpdate(Entity & entity, const UpdateFrame & updateFrame)
{
}

void VoxelClusterSplitSystem::onEvent(
    const VoxelObjectModification & modification)
{
    m_modifiedVoxelObjects.insert(
        modification.entity.component<VoxelObject>().shared_from_this());
}

void VoxelClusterSplitSystem::onCreated()
{
    subscribeEvent<VoxelObjectModification>();
}

void VoxelClusterSplitSystem::onGameUpdate(const UpdateFrame & updateFrame)
{
    auto & voxelWorld = world().systemRef<VoxelWorld>();

    for (auto & object : m_modifiedVoxelObjects)
    {
        auto entity = world().entityById(object->entityId());

        if (!entity.isValid()) continue;

        auto & originalVoxelData = object->data();
        auto & splitDetector = object->data()->splitDetector();

        object->performSplitDetection();

        const auto & segments = splitDetector.segments();

        AssertM(
            !segments.empty() || originalVoxelData->numVoxels() == 0,
            entity.name() +
                ": there has to be one split, the object itself, by "
                "definition. "
                "Remaining voxels: " +
                std::to_string(originalVoxelData->numVoxels()));

        if (segments.size() <= 1) continue;

        for (size_t s = 0; s < segments.size(); s++)
        {
            if (s == splitDetector.splitContainingCrucialVoxel()) continue;
            splitVoxelsOffEntity(entity, segments[s]);
        }
    }
    m_modifiedVoxelObjects.clear();
}

Entity VoxelClusterSplitSystem::splitVoxelsOffEntity(const Entity & originalEntity, const VoxelClusterSegment & segment)
{
    auto & originalVoxelObject = originalEntity.component<VoxelObject>();
    auto & originalVoxelData = originalVoxelObject.data();
    auto & originalBody = originalEntity.component<RigidBodyComponent>().value();

    auto & voxelWorld = originalVoxelObject.voxelWorld();

    std::vector<Voxel> splitVoxels;
    splitVoxels.reserve(segment.voxels.size());

    auto color = RandomColorGenerator().generate();

    for (const auto & voxel : segment.voxels)
    {
        const auto colorIndex =
            originalVoxelData->voxelColorIndex(voxel);
        const auto healthPoints =
            originalVoxelData->voxelHealthPoints(voxel);
        Voxel splitVoxel(voxel - segment.llf, colorIndex, healthPoints);

        splitVoxels.emplace_back(splitVoxel);
    }

    auto splitPalette = std::make_shared<ColorPalette>(
        voxelWorld.drawContext(),
        originalVoxelData->palette()->colors());

    const auto splitSize = segment.urb - segment.llf + 1u;
    auto       splitVoxelData = std::make_shared<VoxelObjectVoxelData>(
        originalVoxelObject.data()->voxelWorld(),
        splitPalette,
        splitSize);
    splitVoxelData->addVoxelsRaw(std::move(splitVoxels));

    auto   splitEntity = originalEntity.world().createEntity("Split");
    auto & splitVoxelObject = splitEntity.addComponent<VoxelObject>();
    splitVoxelObject.setVoxelData(splitVoxelData);

    auto rigidBodyPayload = std::make_shared<VoxelRigidBodyPayload>(
        splitVoxelObject.shared_from_this());
    auto splitBody =
        std::make_shared<RigidBody>(splitVoxelObject.data()->shape());
    splitBody->setEntity(splitEntity);
    splitBody->updateMassProperties();

    auto scale =
        originalEntity.component<Transform3DComponent>().value().scale();

    const auto relativeCenterOfMass =
        (splitBody->shape()->centerOfMass() +
         glm::vec3(segment.llf) /*+ glm::vec3(0.5f) */ -
         originalBody->shape()->centerOfMass()) * scale;

    const auto splitPosition =
        originalBody->transform().position() +
        originalBody->transform().orientation() * (glm::vec3(segment.llf) - originalBody->shape()->centerOfMass());
    //            std::cout << "splitPosition: " << splitPosition <<
    //            std::endl;

    auto & transform =
        splitEntity.addComponent<Transform3DComponent>().value();
   // transform.setCenter(splitBody->shape()->centerOfMass());
    transform.setPosition(splitPosition);
    transform.setOrientation(originalBody->transform().orientation());
    transform.setScale(originalBody->transform().scale());
    splitVoxelObject.setTransform(transform);

    splitBody->setLinearVelocity(originalBody->localVelocity(
        originalBody->transform().pointLocalToWorld(
            originalBody->transform().center() + relativeCenterOfMass) -
        originalBody->transform().position()));
    splitBody->setAngularVelocity(originalBody->angularVelocity());

    splitEntity.addComponent<RigidBodyComponent>(splitBody);

    originalVoxelObject.removeVoxelsRaw(
        segment.voxels);

    return splitEntity;
}