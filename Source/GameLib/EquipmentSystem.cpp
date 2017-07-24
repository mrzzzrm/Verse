#include "EquipmentSystem.h"

#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/World.h>

#include "Equipment.h"
#include "HailstormManager.h"
#include "R.h"
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

void EquipmentSystem::onEntityGameUpdate(Entity & entity, float seconds)
{
    auto   body = entity.component<RigidBodyComponent>().value();
    auto & equipment = entity.component<Equipment>();

    EquipmentUpdateContext equipmentUpdateContext;
    equipmentUpdateContext.targetPose = Pose3D(
        body->transform().position(),
        body->transform().orientation(),
        body->transform().center());
    equipmentUpdateContext.linearVelocity = body->linearVelocity();
    equipmentUpdateContext.angularVelocity = body->angularVelocity();
    equipmentUpdateContext.entity = entity;

    equipment.update(seconds, equipmentUpdateContext);
}