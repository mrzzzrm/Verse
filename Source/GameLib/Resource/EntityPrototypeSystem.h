#pragma once

#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/World.h>

#include "EntityPrototypeManager.h"
#include "GameLib.h"

class EntityPrototypeSystem:
    public System<EntityPrototypeSystem>
{
public:
    EntityPrototypeSystem(World & world):
        Base(world),
        m_manager(world)
    {

    }

    EntityPrototypeManager & manager() { return m_manager; }
    const EntityPrototypeManager & manager() const { return m_manager; }

private:
    EntityPrototypeManager m_manager;
};