#pragma once

#include <string>

#include <Deliberation/Core/Json.h>

#include "Allegiance.h"
#include "ComponentPrototype.h"
#include "GameLib.h"
#include "NpcBehaviour.h"

class AllegiancePrototype:
    public ComponentPrototype
{
public:
    AllegiancePrototype(const Json & json) {
        m_faction = json["Faction"];
    }

    void applyToEntity(Entity & entity) const override
    {
        entity.addComponent<Allegiance>(m_faction);
    }

private:
    std::string m_faction;
};
