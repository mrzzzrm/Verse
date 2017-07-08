#pragma once

#include <string>

#include <Deliberation/ECS/Entity.h>

struct FactionChangeEvent
{
    FactionChangeEvent(Entity entity, const std::string & from, const std::string & to):
        entity(entity),
        from(from),
        to(to)
    {}

    Entity      entity;
    std::string from;
    std::string to;
};