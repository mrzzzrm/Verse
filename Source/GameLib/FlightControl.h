#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "GameLib.h"
#include "FlightControlConfig.h"

namespace deliberation
{

class RigidBody;

}

class FlightControl final
{
public:
    FlightControl(std::shared_ptr<RigidBody> & body, const FlightControlConfig & config);

    void setLinearThrust(const glm::vec3 & linearThrust);
    void setAngularThrust(const glm::vec3 & angularThrust);

    void update(float seconds);

private:
    std::shared_ptr<RigidBody>  m_body;
    FlightControlConfig         m_config;

    glm::vec3                   m_linearThrust;
    glm::vec3                   m_angularThrust;
};