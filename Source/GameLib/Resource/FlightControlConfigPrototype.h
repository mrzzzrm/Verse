#pragma once

#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/ComponentPrototype.h>

#include "FlightControlConfig.h"
#include "GameLib.h"

class FlightControlConfigPrototype
    : public ComponentPrototype<FlightControlConfig>
{
public:
    void updateComponent(const Entity & entity, FlightControlConfig & component) override;
};
