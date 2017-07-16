#pragma once

#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/ComponentPrototype.h>

#include "GameLib.h"
#include "PlayerFlightControl.h"

class PlayerFlightControlPrototype
    : public ComponentPrototype<PlayerFlightControl>
{
public:
    void updateComponent(PlayerFlightControl & playerFlightControl) override;
};
