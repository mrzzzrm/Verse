#pragma once

#include <Deliberation/Core/Json.h>

#include "ComponentPrototype.h"
#include "GameLib.h"
#include "NpcBehaviour.h"

class NpcBehaviourPrototype:
    public ComponentPrototype
{
public:
    NpcBehaviourPrototype(const Json & json) {}

    void applyToEntity(Entity & entity) const override
    {
        entity.addComponent<NpcBehaviour>();
    }
};
