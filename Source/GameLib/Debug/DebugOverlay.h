#pragma once

#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/World.h>

#include "GameLib.h"

class DebugOverlay:
    public System<DebugOverlay>
{
public:
    DebugOverlay(World & world);

    void setFps(float fps);
};