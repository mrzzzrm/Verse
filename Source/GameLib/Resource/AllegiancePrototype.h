#pragma once

#include <string>

#include <Deliberation/Core/Log.h>
#include <Deliberation/Core/Json.h>

#include <Deliberation/ECS/ComponentPrototype.h>

#include "Allegiance.h"
#include "GameLib.h"
#include "NpcBehaviour.h"

class AllegiancePrototype : public ComponentPrototype<Allegiance>
{
public:
    void updateComponent(const Entity & entity, Allegiance & allegiance) override
    {
        DELIBERATION_LOG_INNER_SCOPE("AllegiancePrototype");

        Log->info("Setting faction of '{}': '{}' -> '{}'", entity.name(), allegiance.faction(), m_newJson["Faction"].get<std::string>());

        allegiance.setFaction(m_newJson["Faction"]);
    }
};
