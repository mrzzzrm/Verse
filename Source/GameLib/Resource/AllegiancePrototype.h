#pragma once

#include <string>

#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/ComponentPrototype.h>

#include "Allegiance.h"
#include "GameLib.h"
#include "NpcBehaviour.h"

class AllegiancePrototype:
    public ComponentPrototype<Allegiance>
{
public:
    void updateComponent(Allegiance & allegiance) override
    {
        allegiance.setFaction(m_json["Faction"]);
    }
};
