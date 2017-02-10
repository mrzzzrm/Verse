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
        Base(world, ComponentFilter::requires<std::shared_ptr<NpcController>>())
    {}

protected:
    void onPrePhysicsUpdate(Entity & entity, float seconds) override
    {
        entity.component<std::shared_ptr<NpcController>>()->update(seconds);
    }
};