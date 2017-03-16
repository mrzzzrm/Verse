#pragma once

#include <Deliberation/ECS/System.h>

#include "GameLib.h"

class EquipmentSystem:
    public System<EquipmentSystem>
{
public:
    EquipmentSystem(World & world);

protected:
    void onEntityAdded(Entity & entity) override;
    void onEntityUpdate(Entity & entity, float seconds) override;
};