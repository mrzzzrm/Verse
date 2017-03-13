#pragma once

#include <glm/glm.hpp>

#include <Deliberation/ECS/Component.h>

class CoriolisComponent final:
    public Component<CoriolisComponent>
{
public:
    CoriolisComponent(const glm::vec3 & angularVelocity):
        m_angularVelocity(angularVelocity)
    {}

    const glm::vec3 & angularVelocity() const { return m_angularVelocity; }

private:
    glm::vec3 m_angularVelocity;
};