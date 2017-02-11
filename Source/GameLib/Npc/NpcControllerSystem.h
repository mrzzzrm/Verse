#pragma once

#include <memory>

#include <Deliberation/ECS/System.h>

#include "GameLib.h"
#include "NpcController.h"

class NpcControllerSystem:
    public System<NpcControllerSystem>
{
public:
    NpcControllerSystem(World & world):
        Base(world, ComponentFilter::requires<std::shared_ptr<NpcController>, std::shared_ptr<RigidBody>>())
    {}

protected:
    void onEntityAdded(Entity & entity) override
    {
        auto body = entity.component<std::shared_ptr<RigidBody>>();
        auto npcController = entity.component<std::shared_ptr<NpcController>>();

        npcController->setBody(body);
    }

    void onPrePhysicsUpdate(Entity & entity, float seconds) override
    {
        entity.component<std::shared_ptr<NpcController>>()->update(seconds);
    }
};