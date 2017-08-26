#pragma once

#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/ComponentPrototype.h>

#include "GameLib.h"
#include "HudProxy.h"

class HudProxyPrototype : public ComponentPrototype<HudProxy>
{
public:
    void updateComponent(const Entity & entity, HudProxy & hudProxy) override {}
};
