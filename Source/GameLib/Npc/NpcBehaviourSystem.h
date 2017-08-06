#pragma once

#include <Deliberation/ECS/System.h>

#include "GameLib.h"

class NpcBehaviourSystem : public System<NpcBehaviourSystem>
{
public:
    NpcBehaviourSystem(World & world);

protected:
    void onEntityGameUpdate(Entity & entity, const UpdateFrame & updateFrame);
};