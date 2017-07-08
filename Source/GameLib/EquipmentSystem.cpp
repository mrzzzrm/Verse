#include "EquipmentSystem.h"

#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/World.h>

#include "Equipment.h"
#include "HailstormManager.h"
#include "R.h"
#include "VoxelObject.h"
#include "Weapon.h"

EquipmentSystem::EquipmentSystem(World & world):
    Base(world, ComponentFilter::requires<RigidBodyComponent, Equipment, VoxelObject>())
{
}

void EquipmentSystem::onEvent(const VoxelObjectModification & voxelObjectModification)
{
    auto entity = m_world.entityById(voxelObjectModification.object->entityId());

    if (!entity.hasComponent<Equipment>()) return;

    auto & equipment = entity.component<Equipment>();

    for (const auto & voxel : voxelObjectModification.removals)
    {
        auto it = equipment.m_attachmentByVoxel.find(voxel);
        if (it == equipment.m_attachmentByVoxel.end()) continue;

        it->second->setEnabled(false);
    }
}

void EquipmentSystem::onCreated()
{
    subscribeEvent<VoxelObjectModification>();
}

void EquipmentSystem::onEntityAdded(Entity & entity)
{
    auto & hailstorm = world().systemRef<HailstormManager>();
    auto & equipment = entity.component<Equipment>();
    auto & voxelObject = entity.component<VoxelObject>();

    WeaponConfig weaponConfig;
    weaponConfig.cooldown = 0.2f;
    weaponConfig.meshID = hailstorm.vfxManager().getOrCreateMeshId(R::BulletMesh);
    weaponConfig.bulletSpeed = 400.0f;
    weaponConfig.bulletLifetime = 2.0f;

    for (size_t h = 0; h < equipment.hardpoints().size(); h++)
    {
        auto weapon = std::make_shared<Weapon>(weaponConfig, hailstorm, voxelObject.id().worldUID);
        equipment.setWeapon(h, weapon);
    }
}

void EquipmentSystem::onEntityUpdate(Entity & entity, float seconds)
{
    auto body = entity.component<RigidBodyComponent>().value();
    auto & equipment = entity.component<Equipment>();

    EquipmentUpdateContext equipmentUpdateContext;
    equipmentUpdateContext.targetPose = Pose3D(body->transform().position(),
                                               body->transform().orientation(),
                                               body->transform().center());
    equipmentUpdateContext.linearVelocity = body->linearVelocity();
    equipmentUpdateContext.angularVelocity = body->angularVelocity();
    equipmentUpdateContext.entity = entity;

    equipment.update(seconds, equipmentUpdateContext);
}