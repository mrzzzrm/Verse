#pragma once

#include <Deliberation/ECS/ComponentPrototype.h>

#include "HullComponent.h"
#include "GameLib.h"

class HullPrototype:
    public ComponentPrototype<HullComponent>
{
public:
    void updateComponent(HullComponent & hull)
    {
        hull.setHealth(m_newJson["Health"]);
    }
};