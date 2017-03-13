#pragma once

#include <Deliberation/ECS/System.h>

#include "GameLib.h"

class EquipmentSystem:
    public System<EquipmentSystem>
{
public:
    EquipmentSystem(World & world);

protected:
    void onEntityUpdate(Entity & entity, float seconds);
};