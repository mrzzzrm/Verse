#include "EquipmentSystem.h"

#include <Deliberation/ECS/Components.h>

#include "Equipment.h"

EquipmentSystem::EquipmentSystem(World & world):
    Base(world, ComponentFilter::requires<RigidBodyComponent, Equipment>())
{}

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