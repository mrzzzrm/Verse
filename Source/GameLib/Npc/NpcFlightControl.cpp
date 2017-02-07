#include "NpcFlightControl.h"

#include <cmath>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Core/Math/FloatUtils.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Physics/RigidBody.h>

NpcFlightControl::NpcFlightControl(std::shared_ptr<RigidBody> & body, const FlightControlConfig & config):
    FlightControlBase(body, config)
{}

const glm::vec3 & NpcFlightControl::localLinearAcceleration() const
{
    return m_localLinearAcceleration;
}

const glm::vec3 & NpcFlightControl::localAngularAccelertion() const
{
    return m_localAngularAccelertion;
}

void NpcFlightControl::setLocalLinearAcceleration(const glm::vec3 & localLinearAcceleration)
{
    m_localLinearAcceleration = localLinearAcceleration;
}

void NpcFlightControl::setLocalAngularAccceleration(const glm::vec3 & localAngularAccelertion)
{
    m_localAngularAccelertion = localAngularAccelertion;
}

void NpcFlightControl::update(float seconds)
{
    /**
     * Correct linear acceleration
     */
    const auto localLinearVelocity = m_body->transform().directionWorldToLocal(m_body->linearVelocity());

    const auto correctLinearAccelerationOnAxis = [&] (
        const float currentSpeed, float & currentAcceleration,
        const float maxSpeed, const float maxAcceleration)
    {
        if (EpsilonGt(std::abs(currentSpeed), maxSpeed))
        {
            const auto requiredCorrection = std::abs(currentSpeed) - maxSpeed;
            const auto feasibleCorrection = maxAcceleration * seconds;
            const auto appliedCorrectionFactor = std::min(requiredCorrection / feasibleCorrection, 1.0f);

            currentAcceleration = appliedCorrectionFactor *
                maxAcceleration *
                -Sign(currentSpeed);
        }
    };

    correctLinearAccelerationOnAxis(localLinearVelocity.x, m_localLinearAcceleration.x,
                                    m_config.horizontal.maxSpeed, m_config.horizontal.acceleration);

    correctLinearAccelerationOnAxis(localLinearVelocity.y, m_localLinearAcceleration.y,
                                    m_config.vertical.maxSpeed, m_config.vertical.acceleration);

    if (localLinearVelocity.z > 0.0f)
    {
        correctLinearAccelerationOnAxis(localLinearVelocity.z, m_localLinearAcceleration.z,
                                        m_config.backward.maxSpeed, m_config.forward.acceleration);
    }
    else
    {
        correctLinearAccelerationOnAxis(localLinearVelocity.z, m_localLinearAcceleration.z,
                                        m_config.forward.maxSpeed, m_config.backward.acceleration);
    }

    /**
     * Correct angular acceleration
     */
    const auto localAngularVelocity =  m_body->transform().directionWorldToLocal(m_body->angularVelocity());
    const auto localAngularSpeed = glm::length(localAngularVelocity);

    if (EpsilonGt(localAngularSpeed, 0.0f) && EpsilonGt(localAngularSpeed, m_config.angular.maxSpeed))
    {
        const auto requiredCorrection = localAngularSpeed - m_config.angular.maxSpeed;
        const auto feasibleCorrection = m_config.angular.acceleration * seconds;
        const auto appliedCorrectionFactor = std::min(requiredCorrection / feasibleCorrection, 1.0f);

        m_localAngularAccelertion = -glm::normalize(localAngularVelocity) * appliedCorrectionFactor *
            feasibleCorrection;
    }

    /**
     * Apply accelerations
     */
    m_body->setLinearVelocity(m_body->linearVelocity() + m_body->transform().directionLocalToWorld(m_localLinearAcceleration) * seconds);
    m_body->setAngularVelocity(m_body->angularVelocity() + m_body->transform().directionLocalToWorld(m_localAngularAccelertion) * seconds);
}