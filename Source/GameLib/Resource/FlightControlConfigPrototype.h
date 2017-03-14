#pragma once

#include <Deliberation/Core/Json.h>

#include "GameLib.h"
#include "ComponentPrototype.h"
#include "FlightControlConfig.h"

class FlightControlConfigPrototype:
    public ComponentPrototype
{
public:
    FlightControlConfigPrototype(const Json & json);

    void applyToEntity(Entity & entity) const override;

private:
    FlightControlConfig m_config;
};
