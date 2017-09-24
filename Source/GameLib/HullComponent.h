#pragma once

#include <Deliberation/ECS/Component.h>
#include <Deliberation/ECS/ComponentPrototype.h>

#include <Deliberation/ImGui/ImGui.h>

#include "GameLib.h"

class HullComponent : public Component<HullComponent>
{
DELIBERATION_COMPONENT_NAME("HullComponent")

public:
    float health() const { return m_health; }
    void  setHealth(float health) { m_health = health; }

    void renderImGui() { ImGuiProperty(m_health, "Health"); }

private:
    float m_health = 0.0f;
};

class HullComponentPrototype : public ComponentPrototype<HullComponent>
{
public:
    void updateComponent(const Entity & entity, HullComponent & hull) override
    {
        hull.setHealth(m_newJson["MaxHealth"]);
    }
};