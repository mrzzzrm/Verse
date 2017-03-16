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
    NpcControllerSystem(World & world);

protected:
    void onEntityAdded(Entity & entity) override;
    void onEntityPrePhysicsUpdate(Entity & entity, float seconds) override;
};