#pragma once

#include <Deliberation/ECS/System.h>

#include "GameLib.h"

class HullSystem:
    public System<HullSystem>
{
public:
    HullSystem(World & world);

    void onCreated() override;

    void onEvent(const VoxelObjectBulletHit & hit);
};