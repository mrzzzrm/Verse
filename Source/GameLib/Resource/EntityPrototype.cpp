#include "EntityPrototype.h"

#include "ComponentPrototype.h"

EntityPrototype::EntityPrototype(World & world, const std::string & name):
    m_world(world),
    m_name(name)
{}

void EntityPrototype::addComponentPrototype(const std::shared_ptr<ComponentPrototype> & componentPrototype)
{
    m_componentPrototypes.emplace_back(componentPrototype);
}

Entity EntityPrototype::createEntity(const std::string & name)
{
    auto entity = m_world.createEntity(name);

    for (const auto & componentPrototype : m_componentPrototypes)
    {
        componentPrototype->applyToEntity(entity);
    }

    return entity;
}