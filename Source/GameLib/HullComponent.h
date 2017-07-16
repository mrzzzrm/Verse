#pragma once

#include <Deliberation/ECS/Component.h>

#include "GameLib.h"

class HullComponent : public Component<HullComponent>
{
public:
    float health() const { return m_health; }
    void  setHealth(float health) { m_health = health; }

private:
    float m_health = 0.0f;
};