#pragma once

#include "GameLib.h"

class ComponentPrototype
{
public:
    virtual ~ComponentPrototype() = default;

    virtual void applyToEntity(Entity & entity) const = 0;
};