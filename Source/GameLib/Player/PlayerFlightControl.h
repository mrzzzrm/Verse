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

class PlayerFlightControl final:
    public FlightControlBase
{
public:
    PlayerFlightControl(std::shared_ptr<RigidBody> & body, const FlightControlConfig & config);

    const glm::vec3 & linearThrust() const;
    const glm::vec3 & angularThrust() const;

    void setLinearThrust(const glm::vec3 & linearThrust);
    void setAngularThrust(const glm::vec3 & angularThrust);

    void update(float seconds) override;

private:
    glm::vec3 m_linearThrust;
    glm::vec3 m_angularThrust;
};