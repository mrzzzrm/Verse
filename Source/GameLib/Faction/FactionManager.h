#pragma once

#include <unordered_map>

#include <Deliberation/ECS/System.h>

#include "GameLib.h"

class FactionChangeEvent;

enum class FactionRelation
{
    Friendly,
    Neutral,
    Hostile
};

class FactionManager : public System<FactionManager>
{
public:
    FactionManager(World & world);

    FactionRelation factionRelation(
        const std::string & factionA, const std::string & factionB) const;

    const std::vector<Entity> & faction(const std::string & factionName) const;

    void onEvent(const FactionChangeEvent & event);
    void onCreated() override;

protected:
    void onEntityAdded(Entity & entity) override;
    void onEntityRemoved(Entity & entity) override;

    void
         removeEntityFromFaction(const Entity & entity, const std::string & faction);
    void addEntityToFaction(const Entity & entity, const std::string & faction);

private:
    std::unordered_set<EntityId> m_entities;
    mutable std::unordered_map<std::string, std::vector<Entity>>
        m_entitiesByFaction;
};