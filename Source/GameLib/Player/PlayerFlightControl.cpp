#include "PlayerFlightControl.h"

#include <cmath>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Physics/RigidBody.h>

PlayerFlightControl::PlayerFlightControl(std::shared_ptr<RigidBody> & body, const FlightControlConfig & config):
    FlightControlBase(body, config)
{}

const glm::vec3 & PlayerFlightControl::linearThrust() const
{
    return m_linearThrust;
}

const glm::vec3 & PlayerFlightControl::angularThrust() const
{
    return m_angularThrust;
}

void PlayerFlightControl::setLinearThrust(const glm::vec3 & linearThrust)
{
    Assert(std::abs(linearThrust.x) <= 1.05f && std::abs(linearThrust.y) <= 1.05f && std::abs(linearThrust.z) <= 1.05f,
           "Can't set thrust > 1");

    m_linearThrust = linearThrust;
}

void PlayerFlightControl::setAngularThrust(const glm::vec3 & angularThrust)
{
    Assert(std::abs(angularThrust.x) <= 1.05f && std::abs(angularThrust.y) <= 1.05f && std::abs(angularThrust.z) <= 1.05f,
           "Can't set thrust > 1");

    m_angularThrust = angularThrust;
}

void PlayerFlightControl::update(float seconds)
{
    /**
     * Control linear velocity
     */
    auto localLinearVelocity = m_body->transform().directionWorldToLocal(m_body->linearVelocity());

    auto targetLocalLinearVelocity = glm::vec3({
                                                  m_linearThrust.x * m_config.horizontal.maxSpeed,
                                                  m_linearThrust.y * m_config.vertical.maxSpeed,
                                                  m_linearThrust.z * (m_linearThrust.z > 0
                                                                      ? m_config.backward.maxSpeed
                                                                      : m_config.forward.maxSpeed)
                                              });

    auto deltaLocalLinearVelocity = targetLocalLinearVelocity - localLinearVelocity;

    glm::vec3 localLinearAcceleration;
    if (std::fabs(deltaLocalLinearVelocity.x) > m_config.horizontal.acceleration * seconds)
    {
        localLinearAcceleration.x = m_config.horizontal.acceleration * seconds * SigNum(deltaLocalLinearVelocity.x);
    }
    else
    {
        localLinearAcceleration.x = deltaLocalLinearVelocity.x;
    }

    if (std::fabs(deltaLocalLinearVelocity.y) > m_config.vertical.acceleration * seconds)
    {
        localLinearAcceleration.y = m_config.vertical.acceleration * seconds * SigNum(deltaLocalLinearVelocity.y);
    }
    else
    {
        localLinearAcceleration.y = deltaLocalLinearVelocity.y;
    }

    if (deltaLocalLinearVelocity.z > 0)
    {
        if (deltaLocalLinearVelocity.z > m_config.backward.acceleration * seconds)
        {
            localLinearAcceleration.z = m_config.backward.acceleration * seconds;
        }
        else
        {
            localLinearAcceleration.z = deltaLocalLinearVelocity.z;
        }
    }
    else
    {
        if (-deltaLocalLinearVelocity.z > m_config.forward.acceleration * seconds)
        {
            localLinearAcceleration.z = -m_config.forward.acceleration * seconds;
        }
        else
        {
            localLinearAcceleration.z = -deltaLocalLinearVelocity.z;
        }
    }

    m_body->setLinearVelocity(m_body->linearVelocity() + m_body->transform().directionLocalToWorld(localLinearAcceleration));

    /**
     * Control angular velocity
     */
    auto localAngularVelocity = m_body->transform().directionWorldToLocal(m_body->angularVelocity());

    auto targetLocalAngularVelocity = m_angularThrust * m_config.angular.maxSpeed;

    auto deltaLocalAngularVelocity = targetLocalAngularVelocity - localAngularVelocity;

    glm::vec3 localAngularAcceleration;
    if (glm::length(deltaLocalAngularVelocity) > m_config.angular.acceleration * seconds)
    {
        localAngularAcceleration = glm::normalize(deltaLocalAngularVelocity) * m_config.angular.acceleration * seconds;
    }
    else
    {
        localAngularAcceleration = deltaLocalAngularVelocity;
    }

    m_body->setAngularVelocity(m_body->angularVelocity() + m_body->transform().directionLocalToWorld(localAngularAcceleration));
}