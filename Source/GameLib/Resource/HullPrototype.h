#pragma once

#include <Deliberation/ECS/ComponentPrototype.h>

#include "GameLib.h"
#include "HullComponent.h"

class HullPrototype : public ComponentPrototype<HullComponent>
{
public:
    void updateComponent(const Entity & entity, HullComponent & hull)
    {
        hull.setHealth(m_newJson["Health"]);
    }
};