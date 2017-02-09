#pragma once

#include <memory>

#include <Deliberation/ECS/System.h>

#include "GameLib.h"
#include "NpcController.h"

class NpcFlightControlSystem:
    public System<NpcFlightControlSystem>
{
public:
    NpcFlightControlSystem(World & world):
        Base(world, ComponentFilter::requires<std::shared_ptr<NpcFlightControl>>())
    {}

protected:
    void onUpdate(Entity & entity, float seconds)
    {
        entity.component<std::shared_ptr<NpcFlightControlSystem>>()->update(seconds);
    }
};