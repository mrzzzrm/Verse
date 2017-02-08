#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "GameLib.h"
#include "FlightControlBase.h"
#include "FlightControlConfig.h"

namespace deliberation
{

class RigidBody;

}

class NpcFlightControl final:
    public FlightControlBase
{
public:
    NpcFlightControl(std::shared_ptr<RigidBody> & body, const FlightControlConfig & config);

    const glm::vec3 & localLinearAcceleration() const;
    const glm::vec3 & localAngularAccelertion() const;

    void setLocalLinearAcceleration(const glm::vec3 & localLinearAcceleration);
    void setLocalAngularAccceleration(const glm::vec3 & localAngularAccelertion);

    void update(float seconds) override;

    glm::vec3 correctiveAcceleration(float requiredCorretion, float acceleration, float seconds,
                                     const glm::vec3 & direction) const;

private:
    glm::vec3 m_localLinearAcceleration;
    glm::vec3 m_localAngularAccelertion;
};