#pragma once

#include <memory>
#include <string>

#include <Deliberation/ECS/World.h>

#include "GameLib.h"

class ComponentPrototype;

class EntityPrototype final
{
public:
    EntityPrototype(World & world, const std::string & name);

    void addComponentPrototype(const std::shared_ptr<ComponentPrototype> & componentPrototype);

    Entity createEntity(const std::string & name);

private:
    World &                                             m_world;
    std::string                                         m_name;
    std::vector<std::shared_ptr<ComponentPrototype>>    m_componentPrototypes;
};