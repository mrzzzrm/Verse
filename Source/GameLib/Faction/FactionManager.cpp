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

    auto & fromFaction = m_entitiesByFaction[event.from];
    auto iter = std::find(fromFaction.begin(), fromFaction.end(), event.entity);
    Assert(iter != fromFaction.end(), "");

    fromFaction.erase(iter);

    m_entitiesByFaction[event.to].emplace_back(event.entity);
}

void FactionManager::onCreated()
{
    subscribeEvent<FactionChangeEvent>();
}

void FactionManager::onEntityAdded(Entity & entity)
{
    const auto & allegiance = entity.component<Allegiance>();
    m_entitiesByFaction[allegiance.faction()].emplace_back(entity);
    std::cout << "onEntityAdded " << allegiance.faction() << " " << entity.name() << std::endl;
}

void FactionManager::onEntityRemoved(Entity & entity)
{
    const auto & allegiance = entity.component<Allegiance>();
    auto & faction = m_entitiesByFaction[allegiance.faction()];

    std::cout << "onEntityRemoved " << entity.name() << " " << allegiance.faction() << std::endl;
    for (auto & pair : m_entitiesByFaction)
    {
        std::cout <<  " " <<  pair.first << ": " << std::endl;
        for (auto & entity : pair.second)
        {
            std::cout << "  " << entity.name() << std::endl;
        }
    }

    const auto iter = std::find(faction.begin(), faction.end(), entity);
    Assert(iter != faction.end(), "No such entity '" + entity.name() + "'");

    faction.erase(iter);
}
