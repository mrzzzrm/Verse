#include "EquipmentSystem.h"

#include <Deliberation/ECS/RigidBodyComponent.h>
#include <Deliberation/ECS/World.h>

#include "Equipment.h"
#include "HailstormManager.h"
#include "VoxelObject.h"
#include "Weapon.h"

EquipmentSystem::EquipmentSystem(World & world)
    : Base(
          world,
          ComponentFilter::
              requires<RigidBodyComponent, Equipment, VoxelObject>())
{
}

void EquipmentSystem::onEvent(
    const VoxelObjectModification & voxelObjectModification)
{
    auto entity = voxelObjectModification.entity;

    if (!entity.hasComponent<Equipment>()) return;

    auto & equipment = entity.component<Equipment>();

    auto forEachRemoval = [&](auto & voxel) {
        auto it = equipment.m_attachmentByVoxel.find(voxel);
        if (it == equipment.m_attachmentByVoxel.end()) return;

        it->second->setEnabled(false);
    };

    for (const auto & voxel : voxelObjectModification.destructions)
        forEachRemoval(voxel);
    for (const auto & voxel : voxelObjectModification.splits)
        forEachRemoval(voxel);
}

void EquipmentSystem::onCreated() { subscribeEvent<VoxelObjectModification>(); }

void EquipmentSystem::onEntityAdded(Entity & entity)
{
}

void EquipmentSystem::onEntityRemoved(Entity & entity)
{
    auto & equipment = entity.component<Equipment>();

    for (auto & attachment : equipment.attachments())
    {
        attachment->setEnabled(false);
    }
}

void EquipmentSystem::onEntityPostPhysicsUpdate(Entity & entity, const UpdateFrame & updateFrame)
{
    auto & equipment = entity.component<Equipment>();
    equipment.postPhysicsUpdate(updateFrame);
}

void EquipmentSystem::onEntityGameUpdate(Entity & entity, const UpdateFrame & updateFrame)
{
    auto   body = entity.component<RigidBodyComponent>().value();
    auto & equipment = entity.component<Equipment>();

    EquipmentUpdateContext equipmentUpdateContext;
    equipmentUpdateContext.body = body;
    equipmentUpdateContext.entity = entity;

    equipment.gameUpdate(updateFrame, equipmentUpdateContext);
}