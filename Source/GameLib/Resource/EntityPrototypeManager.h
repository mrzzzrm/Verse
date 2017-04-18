#pragma once

#include <unordered_map>

#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/World.h>

#include "GameLib.h"

class ComponentPrototype;
struct EntityDesc;
class EntityPrototype;

class EntityPrototypeManager final
{
public:
    EntityPrototypeManager(World & world);

    Entity createEntity(const EntityDesc & desc);

private:
    template<typename T, typename ... Args>
    void registerComponentLoader(const std::string & name, Args &&... args);

private:
    World &                         m_world;
    std::unordered_map<std::string, std::shared_ptr<EntityPrototype>>
                                    m_entityPrototypeByName;
    std::unordered_map<std::string, std::function<std::shared_ptr<ComponentPrototype>(const Json & json)>>
                                    m_componentPrototypeLoaderByType;
};