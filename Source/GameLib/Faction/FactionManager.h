#pragma once

#include <unordered_map>

#include <Deliberation/ECS/System.h>

#include "GameLib.h"

class FactionManager:
    public System<FactionManager>
{
public:
    FactionManager(World & world);

    const std::vector<Entity> & faction(const std::string & factionName) const;

protected:
    void onEntityAdded(Entity & entity) override;
    void onEntityRemoved(Entity & entity) override;

private:
    mutable std::unordered_map<std::string,
        std::vector<Entity>> m_entitiesByFaction;
};