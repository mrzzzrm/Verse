#pragma once

#include <Deliberation/ECS/System.h>

#include "GameLib.h"

class VoxelClusterSplitSystem:
    public System<VoxelClusterSplitSystem>
{
public:
    VoxelClusterSplitSystem(World & world);

protected:
    virtual void onUpdate(Entity & entity, float seconds);
};