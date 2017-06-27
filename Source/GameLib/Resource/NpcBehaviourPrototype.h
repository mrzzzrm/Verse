#pragma once

#include <Deliberation/Core/Json.h>

#include "GameLib.h"
#include "NpcBehaviour.h"

class NpcBehaviourPrototype:
    public ComponentPrototype<NpcBehaviour>
{
public:
    void updateComponent(NpcBehaviour &) override
    {
    }
};
