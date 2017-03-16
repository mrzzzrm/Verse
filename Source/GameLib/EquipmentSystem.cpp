#include "EquipmentSystem.h"

#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/World.h>

#include "Equipment.h"
#include "HailstormManager.h"
#include "VoxelObject.h"
#include "Weapon.h"

EquipmentSystem::EquipmentSystem(World & world):
    Base(world, ComponentFilter::requires<RigidBodyComponent, Equipment, VoxelObject>())
{}

void EquipmentSystem::onEntityAdded(Entity & entity)
{
    auto & hailstorm = world().system<HailstormManager>();
    auto & equipment = entity.component<Equipment>();
    auto & voxelObject = entity.component<VoxelObject>();

    WeaponConfig weaponConfig;
    weaponConfig.cooldown = 0.2f;
    weaponConfig.meshID = 0;

    std::cout << "Num Hardpoints: " << equipment.numHardpoints() << std::endl;

    for (size_t h = 0; h < equipment.numHardpoints(); h++)
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

    equipment.update(seconds, equipmentUpdateContext);
}