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

void FactionManager::onEntityAdded(Entity & entity)
{
    const auto & allegiance = entity.component<Allegiance>();
    m_entitiesByFaction[allegiance.faction()].emplace_back(entity);
}

void FactionManager::onEntityRemoved(Entity & entity)
{
    const auto & allegiance = entity.component<Allegiance>();
    auto & faction = m_entitiesByFaction[allegiance.faction()];

    const auto iter = std::find(faction.begin(), faction.end(), entity);
    Assert(iter != faction.end(), "");

    faction.erase(iter);
}
