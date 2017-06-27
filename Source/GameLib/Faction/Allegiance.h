#pragma once

#include <Deliberation/ECS/Component.h>

#include "GameLib.h"

class Allegiance:
    public Component<Allegiance>
{
public:
    const std::string & faction() const;
    void setFaction(const std::string & faction) { m_faction = faction; }

private:
    std::string m_faction;
};