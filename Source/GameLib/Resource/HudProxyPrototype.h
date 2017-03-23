#pragma once

#include <Deliberation/Core/Json.h>

#include "GameLib.h"
#include "ComponentPrototype.h"
#include "HudProxy.h"

class HudProxyPrototype:
    public ComponentPrototype
{
public:
    HudProxyPrototype(const Json & json) {}

    void applyToEntity(Entity & entity) const override {
        entity.addComponent<HudProxy>();
    }
};
