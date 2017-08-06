#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/ECS/Component.h>

#include "FlightControlConfig.h"
#include "GameLib.h"

namespace deliberation
{
class UpdateFrame;
class RigidBody;
}

class NpcFlightControl final : public Component<NpcFlightControl>
{
    DELIBERATION_COMPONENT_NAME("NpcFlightControl")
public:
    const glm::vec3 & localLinearAcceleration() const;
    const glm::vec3 & localAngularAccelertion() const;

    void setLocalLinearAcceleration(const glm::vec3 & localLinearAcceleration);
    void
    setLocalAngularAccceleration(const glm::vec3 & localAngularAccelertion);

    void
    update(RigidBody & body, const FlightControlConfig & config, const UpdateFrame & updateFrame);

    glm::vec3 correctiveAcceleration(
        float             requiredCorretion,
        float             acceleration,
        const UpdateFrame & updateFrame,
        const glm::vec3 & direction) const;

private:
    glm::vec3 m_localLinearAcceleration;
    glm::vec3 m_localAngularAccelertion;
};