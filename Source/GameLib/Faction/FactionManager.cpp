#include "FactionManager.h"

#include <algorithm>

#include <Deliberation/ECS/Components.h>

#include "Allegiance.h"

FactionManager::FactionManager(World & world):
    Base(world, ComponentFilter::requires<Allegiance, RigidBodyComponent>())
{

}

FactionRelation FactionManager::factionRelation(const std::string & factionA, const std::string & factionB) const
{
    return FactionRelation::Hostile;
}

const std::vector<Entity> & FactionManager::faction(const std::string & factionName) const
{
    auto & faction = m_entitiesByFaction[factionName];
    return faction;
}

void FactionManager::onEvent(const FactionChangeEvent & event)
{
    removeEntityFromFaction(event.entity, event.from);
    addEntityToFaction(event.entity, event.to);
}

void FactionManager::onCreated()
{
    subscribeEvent<FactionChangeEvent>();
}

void FactionManager::onEntityAdded(Entity & entity)
{
    const auto & allegiance = entity.component<Allegiance>();
    addEntityToFaction(entity, allegiance.faction());
}

void FactionManager::onEntityRemoved(Entity & entity)
{
    const auto & allegiance = entity.component<Allegiance>();
    removeEntityFromFaction(entity, allegiance.faction());
}

void FactionManager::removeEntityFromFaction(const Entity & entity, const std::string & faction)
{
    auto & entities = m_entitiesByFaction[faction];
    const auto iter = std::find(entities.begin(), entities.end(), entity);
    Assert(iter != entities.end(), "");
    entities.erase(iter);
}

void FactionManager::addEntityToFaction(const Entity & entity, const std::string & faction)
{
    auto & entities = m_entitiesByFaction[faction];
    const auto iter = std::find(entities.begin(), entities.end(), entity);
    Assert(iter == entities.end(), "");
    entities.emplace_back(entity);
}
