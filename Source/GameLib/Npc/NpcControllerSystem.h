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
            std::shared_ptr<NpcController>,
            std::shared_ptr<RigidBody>,
            std::shared_ptr<Equipment>>())
    {}

protected:
    void onEntityAdded(Entity & entity) override
    {
        auto body = entity.component<std::shared_ptr<RigidBody>>();
        auto npcController = entity.component<std::shared_ptr<NpcController>>();

        npcController->setBody(body);

        npcController->setEquipment(entity.component<std::shared_ptr<Equipment>>());
    }

    void onPrePhysicsUpdate(Entity & entity, float seconds) override
    {
        auto body = entity.component<std::shared_ptr<RigidBody>>();

        entity.component<std::shared_ptr<NpcController>>()->update(seconds);

        EquipmentUpdateContext equipmentUpdateContext;
        equipmentUpdateContext.targetPose = Pose3D(body->transform().position(),
                                                   body->transform().orientation());
        equipmentUpdateContext.linearVelocity = body->linearVelocity();
        equipmentUpdateContext.angularVelocity = body->angularVelocity();

        entity.component<std::shared_ptr<Equipment>>()->update(seconds, equipmentUpdateContext);
    }
};