#pragma once

#include <Deliberation/ECS/System.h>

#include "GameLib.h"

class RigidBodyVoxelObjectSystem:
    public System<RigidBodyVoxelObjectSystem>
{
public:
    RigidBodyVoxelObjectSystem(World & world);

protected:
    void onUpdate(Entity & entity, float seconds) override;
};