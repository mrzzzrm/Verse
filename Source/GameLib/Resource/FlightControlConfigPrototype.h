#pragma once

#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/ComponentPrototype.h>

#include "GameLib.h"
#include "FlightControlConfig.h"

class FlightControlConfigPrototype:
    public ComponentPrototype<FlightControlConfig>
{
public:
    void updateComponent(FlightControlConfig & component) override;
};
