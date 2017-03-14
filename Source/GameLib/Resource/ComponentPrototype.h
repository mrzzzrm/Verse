#pragma once

#include "GameLib.h"

namespace deliberation
{
class Entity;
}

class ComponentPrototype
{
public:
    virtual ~ComponentPrototype() = default;

    virtual void applyToEntity(Entity & entity) const = 0;
};