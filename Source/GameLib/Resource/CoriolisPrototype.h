#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Json.h>

#include <Deliberation/ECS/Entity.h>
#include <Deliberation/ECS/ComponentPrototype.h>

#include "CoriolisComponent.h"

class CoriolisPrototype:
    public ComponentPrototype<CoriolisComponent>
{
public:
    void updateComponent(CoriolisComponent & coriolis) override
    {
        coriolis.setAngularVelocity(m_newJson["AngularVelocity"]);
    }
};