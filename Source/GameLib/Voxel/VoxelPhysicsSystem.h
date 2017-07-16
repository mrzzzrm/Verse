#pragma once

#include <Deliberation/ECS/System.h>

#include "GameLib.h"

class VoxelPhysicsSystem : public System<VoxelPhysicsSystem>
{
public:
    VoxelPhysicsSystem(World & world);

protected:
    void onEntityAdded(Entity & entity);
};