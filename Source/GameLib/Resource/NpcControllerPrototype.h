#pragma once

#include <Deliberation/Core/Json.h>

#include "GameLib.h"
#include "ComponentPrototype.h"

class NpcControllerPrototype:
    public ComponentPrototype
{
public:
    NpcControllerPrototype(const Json & json);

    void applyToEntity(Entity & entity) const override;
};
