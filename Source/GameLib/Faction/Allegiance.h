#pragma once

#include <Deliberation/ECS/Component.h>

#include "GameLib.h"

class Allegiance:
    public Component<Allegiance>
{
public:
    Allegiance(const std::string & faction);

    const std::string & faction() const;

private:
    std::string m_faction;
};