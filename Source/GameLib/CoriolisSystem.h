#pragma once

#include <Deliberation/ECS/System.h>

#include "GameLib.h"

class CoriolisSystem : public System<CoriolisSystem>
{
public:
    CoriolisSystem(World & world);

protected:
    void onEntityGameUpdate(Entity & entity, const UpdateFrame & updateFrame) override;
};