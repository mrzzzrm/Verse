#include "PlayerFlightControl.h"

#include <cmath>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Physics/RigidBody.h>

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

void PlayerFlightControl::update(RigidBody & body, FlightControlConfig & config, float seconds)
{
    /**
     * Control linear velocity
     */
    auto localLinearVelocity = body.transform().directionWorldToLocal(body.linearVelocity());

    auto targetLocalLinearVelocity = glm::vec3({
                                                  m_linearThrust.x * config.horizontal.maxSpeed,
                                                  m_linearThrust.y * config.vertical.maxSpeed,
                                                  m_linearThrust.z * (m_linearThrust.z > 0
                                                                      ? config.backward.maxSpeed
                                                                      : config.forward.maxSpeed)
                                              });

    auto deltaLocalLinearVelocity = targetLocalLinearVelocity - localLinearVelocity;

    glm::vec3 localLinearAcceleration;
    if (std::fabs(deltaLocalLinearVelocity.x) > config.horizontal.acceleration * seconds)
    {
        localLinearAcceleration.x = config.horizontal.acceleration * seconds * SigNum(deltaLocalLinearVelocity.x);
    }
    else
    {
        localLinearAcceleration.x = deltaLocalLinearVelocity.x;
    }

    if (std::fabs(deltaLocalLinearVelocity.y) > config.vertical.acceleration * seconds)
    {
        localLinearAcceleration.y = config.vertical.acceleration * seconds * SigNum(deltaLocalLinearVelocity.y);
    }
    else
    {
        localLinearAcceleration.y = deltaLocalLinearVelocity.y;
    }

    if (deltaLocalLinearVelocity.z > 0)
    {
        if (deltaLocalLinearVelocity.z > config.backward.acceleration * seconds)
        {
            localLinearAcceleration.z = config.backward.acceleration * seconds;
        }
        else
        {
            localLinearAcceleration.z = deltaLocalLinearVelocity.z;
        }
    }
    else
    {
        if (-deltaLocalLinearVelocity.z > config.forward.acceleration * seconds)
        {
            localLinearAcceleration.z = -config.forward.acceleration * seconds;
        }
        else
        {
            localLinearAcceleration.z = -deltaLocalLinearVelocity.z;
        }
    }

    body.setLinearVelocity(body.linearVelocity() + body.transform().directionLocalToWorld(localLinearAcceleration));

    /**
     * Control angular velocity
     */
    auto localAngularVelocity = body.transform().directionWorldToLocal(body.angularVelocity());

    auto targetLocalAngularVelocity = m_angularThrust * config.angular.maxSpeed;

    auto deltaLocalAngularVelocity = targetLocalAngularVelocity - localAngularVelocity;

    glm::vec3 localAngularAcceleration;
    if (glm::length(deltaLocalAngularVelocity) > config.angular.acceleration * seconds)
    {
        localAngularAcceleration = glm::normalize(deltaLocalAngularVelocity) * config.angular.acceleration * seconds;
    }
    else
    {
        localAngularAcceleration = deltaLocalAngularVelocity;
    }

    body.setAngularVelocity(body.angularVelocity() + body.transform().directionLocalToWorld(localAngularAcceleration));
}