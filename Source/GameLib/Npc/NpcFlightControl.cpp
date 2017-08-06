#include "NpcFlightControl.h"

#include <cmath>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/UpdateFrame.h>
#include <Deliberation/Core/Math/FloatUtils.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Physics/RigidBody.h>

const glm::vec3 & NpcFlightControl::localLinearAcceleration() const
{
    return m_localLinearAcceleration;
}

const glm::vec3 & NpcFlightControl::localAngularAccelertion() const
{
    return m_localAngularAccelertion;
}

void NpcFlightControl::setLocalLinearAcceleration(
    const glm::vec3 & localLinearAcceleration)
{
    m_localLinearAcceleration = localLinearAcceleration;
}

void NpcFlightControl::setLocalAngularAccceleration(
    const glm::vec3 & localAngularAccelertion)
{
    m_localAngularAccelertion = localAngularAccelertion;
}

void NpcFlightControl::update(
    RigidBody & body, const FlightControlConfig & config, const UpdateFrame & updateFrame)
{
    /**
     * Correct linear acceleration
     */
    const auto localLinearVelocity =
        body.transform().directionWorldToLocal(body.linearVelocity());

    const auto correctLinearAccelerationOnAxis =
        [&](const float currentSpeed,
            float &     currentAcceleration,
            const float maxSpeed,
            const float maxAcceleration) {
            if (EpsilonGt(std::abs(currentSpeed), maxSpeed))
            {
                const auto requiredCorrection =
                    std::abs(currentSpeed) - maxSpeed;
                const auto feasibleCorrection = maxAcceleration * updateFrame.gameSeconds();
                const auto appliedCorrectionFactor =
                    std::min(requiredCorrection / feasibleCorrection, 1.0f);

                currentAcceleration = appliedCorrectionFactor *
                                      maxAcceleration * -Sign(currentSpeed);
            }
        };

    correctLinearAccelerationOnAxis(
        localLinearVelocity.x,
        m_localLinearAcceleration.x,
        config.horizontal.maxSpeed,
        config.horizontal.acceleration);

    correctLinearAccelerationOnAxis(
        localLinearVelocity.y,
        m_localLinearAcceleration.y,
        config.vertical.maxSpeed,
        config.vertical.acceleration);

    if (localLinearVelocity.z > 0.0f)
    {
        correctLinearAccelerationOnAxis(
            localLinearVelocity.z,
            m_localLinearAcceleration.z,
            config.backward.maxSpeed,
            config.forward.acceleration);
    }
    else
    {
        correctLinearAccelerationOnAxis(
            localLinearVelocity.z,
            m_localLinearAcceleration.z,
            config.forward.maxSpeed,
            config.backward.acceleration);
    }

    /**
     * Correct angular acceleration
     */
    const auto localAngularVelocity =
        body.transform().directionWorldToLocal(body.angularVelocity());
    const auto localAngularSpeed = glm::length(localAngularVelocity);

    if (EpsilonGt(localAngularSpeed, 0.0f) &&
        EpsilonGt(localAngularSpeed, config.angular.maxSpeed))
    {
        m_localAngularAccelertion = correctiveAcceleration(
            localAngularSpeed - config.angular.maxSpeed,
            config.angular.acceleration,
            updateFrame,
            -glm::normalize(localAngularVelocity));
    }

    /**
     * Apply accelerations
     */
    body.setLinearVelocity(
        body.linearVelocity() +
        body.transform().directionLocalToWorld(m_localLinearAcceleration) *
            updateFrame.gameSeconds());
    body.setAngularVelocity(
        body.angularVelocity() +
        body.transform().directionLocalToWorld(m_localAngularAccelertion) *
            updateFrame.gameSeconds());
}

glm::vec3 NpcFlightControl::correctiveAcceleration(
    float             requiredCorrection,
    float             acceleration,
    const UpdateFrame & updateFrame,
    const glm::vec3 & direction) const
{
    const auto feasibleCorrection = acceleration * updateFrame.gameSeconds();
    const auto appliedCorrectionFactor =
        std::min(requiredCorrection / feasibleCorrection, 1.0f);

    return direction * appliedCorrectionFactor * acceleration;
}