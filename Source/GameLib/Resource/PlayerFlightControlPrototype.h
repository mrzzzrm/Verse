#pragma once

#include <Deliberation/Core/Json.h>

#include "GameLib.h"
#include "ComponentPrototype.h"

class PlayerFlightControlPrototype:
    public ComponentPrototype
{
public:
    PlayerFlightControlPrototype(const Json & json);

    void applyToEntity(Entity & entity) const override;
};
