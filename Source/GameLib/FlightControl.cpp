#include "FlightControl.h"

#include <cmath>

#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Physics/RigidBody.h>

FlightControl::FlightControl(std::shared_ptr<RigidBody> & body, const FlightControlConfig & config):
    m_body(body),
    m_config(config)
{}

const std::shared_ptr<RigidBody> & FlightControl::body() const
{
    return m_body;
}

const FlightControlConfig & FlightControl::config() const
{
    return m_config;
}

void FlightControl::setLinearThrust(const glm::vec3 & linearThrust)
{
    m_linearThrust = linearThrust;
}

void FlightControl::setAngularThrust(const glm::vec3 & angularThrust)
{
    m_angularThrust = angularThrust;
}

void FlightControl::update(float seconds)
{
    /**
     * Control linear velocity
     */
    auto localLinearVelocity = m_body->transform().directionWorldToLocal(m_body->linearVelocity());

    auto targetLocalLinearVelocity = glm::vec3({
                                                  m_linearThrust.x *
                                                  m_config.horizontal.maxSpeed,
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