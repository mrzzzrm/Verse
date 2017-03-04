#pragma once

#include <memory>

#include <Deliberation/ECS/System.h>

#include "Equipment.h"
#include "GameLib.h"
#include "NpcController.h"

class NpcControllerSystem:
    public System<NpcControllerSystem>
{
public:
    NpcControllerSystem(World & world):
        Base(world, ComponentFilter::requires<
            NpcController,
            RigidBodyComponent,
            Equipment>())
    {}

protected:
    void onEntityAdded(Entity & entity) override
    {
        auto body = entity.component<RigidBodyComponent>().value();
        auto & npcController = entity.component<NpcController>();

        npcController.setBody(body);
    }

    void onPrePhysicsUpdate(Entity & entity, float seconds) override
    {
        auto body = entity.component<RigidBodyComponent>().value();
        auto & equipment = entity.component<Equipment>();

        entity.component<NpcController>().update(seconds, equipment);

        EquipmentUpdateContext equipmentUpdateContext;
        equipmentUpdateContext.targetPose = Pose3D(body->transform().position(),
                                                   body->transform().orientation(),
                                                   body->transform().center());
        equipmentUpdateContext.linearVelocity = body->linearVelocity();
        equipmentUpdateContext.angularVelocity = body->angularVelocity();

        equipment.update(seconds, equipmentUpdateContext);
    }
};