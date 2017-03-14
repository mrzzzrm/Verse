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

void EntityPrototype::applyToEntity(Entity & entity)
{
    for (const auto & componentPrototype : m_componentPrototypes)
    {
        componentPrototype->applyToEntity(entity);
    }
}