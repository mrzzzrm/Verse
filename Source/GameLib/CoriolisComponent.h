#pragma once

#include <glm/glm.hpp>

#include <Deliberation/ECS/Component.h>

class CoriolisComponent final : public Component<CoriolisComponent>
{
    DELIBERATION_COMPONENT_NAME("Coriolis")
public:
    const glm::vec3 & angularVelocity() const { return m_angularVelocity; }
    void              setAngularVelocity(const glm::vec3 & angularVelocity)
    {
        m_angularVelocity = angularVelocity;
    }

private:
    glm::vec3 m_angularVelocity;
};