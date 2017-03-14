#pragma once

#include <memory>

#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/System.h>

#include "Equipment.h"
#include "FlightControlConfig.h"
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
            Equipment,
            NpcFlightControl,
            FlightControlConfig>())
    {}

protected:
    void onEntityAdded(Entity & entity) override
    {
        entity.addComponent<NpcFlightControl>();
    }

    void onEntityPrePhysicsUpdate(Entity & entity, float seconds) override
    {
        auto & body = *entity.component<RigidBodyComponent>().value();
        auto & equipment = entity.component<Equipment>();
        auto & npcController = entity.component<NpcController>();
        auto & flightControl = entity.component<NpcFlightControl>();
        auto & flightControlConfig = entity.component<FlightControlConfig>();

        auto & task = npcController.task();

        if (task) task->update(npcController, body, equipment, seconds);

        npcController.update(body, flightControl, flightControlConfig, seconds);
        flightControl.update(body, flightControlConfig, seconds);
    }
};