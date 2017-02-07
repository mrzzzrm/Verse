#include "FlyToTask.h"

#include <algorithm>
#include <iostream>

#include <glm/gtx/vector_angle.hpp>

#include <Deliberation/Core/Math/FloatUtils.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Pose3D.h>
#include <Deliberation/Core/Math/Random.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Physics/RigidBody.h>

FlyToTask::FlyToTask(std::shared_ptr<NpcFlightControl> flightControl, const glm::vec3 & destination):
    NpcTask(flightControl),
    m_destination(destination)
{

}

const glm::vec3 & FlyToTask::destination() const
{
    return m_destination;
}

void FlyToTask::setDestination(const glm::vec3 & destination)
{
    m_destination = destination;
}

void FlyToTask::update(float seconds)
{
    auto & body = m_flightControl->body();

    Transform3D predictedPose;
    body->predictTransform(seconds, predictedPose);

    auto linearDeltaToDestination = m_destination - predictedPose.position();
    auto localLinearDeltaToDestination = predictedPose.directionWorldToLocal(linearDeltaToDestination);
    auto linearDistanceToDestination = glm::length(linearDeltaToDestination);
    auto localLinearVelocity = predictedPose.directionWorldToLocal(body->linearVelocity());

    if (EpsilonEq(linearDistanceToDestination, 0.0f))
    {
        m_flightControl->setLocalLinearAcceleration(glm::vec3());
        m_flightControl->setLocalAngularAccceleration(glm::vec3());
        return;
    }

    auto localDirectionToDestination = glm::normalize(localLinearDeltaToDestination);

    /**
     * Angular
     */
    auto angularDeltaToDestination = glm::angle(localDirectionToDestination,
                                                glm::vec3(0.0f, 0.0f, -1.0f));


    glm::vec3 idealRotationAxis;

    if (std::abs(angularDeltaToDestination - glm::pi<float>()) < 0.05)
    {
        // any axis will do
        idealRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    else
    {
        idealRotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), localDirectionToDestination);
    }

    auto angleToDestination = std::asin(glm::length(idealRotationAxis));

//    if (angleToDestination < 0.1f)
//    {
//        auto destination = RandomInHemisphere({0.0f, 300.0f, 0.0f});
//        setDestination(destination);
//    }

    auto idealAngularSpeed =
        std::min(std::sqrt(2 * m_flightControl->config().angular.acceleration * angleToDestination),
                 m_flightControl->config().angular.maxSpeed);

    auto idealAngularVelocity = glm::normalize(idealRotationAxis) * idealAngularSpeed;

    auto idealAngularVelocityCorrection = idealAngularVelocity - body->angularVelocity();

    if (glm::length(idealAngularVelocityCorrection) > 0.001f)
    {
        m_flightControl->setLocalAngularAccceleration(glm::normalize(idealAngularVelocityCorrection) * 0.3f);
    }
    else
    {
        m_flightControl->setLocalAngularAccceleration(glm::vec3(0.0f));
    }

    /**
     * Linear
     */
    const auto directionControl = m_flightControl->config().direction(localDirectionToDestination);

    float idealSpeed;
    if (angleToDestination < 0.5f)
    {
        idealSpeed = std::min(std::sqrt(2 * directionControl.acceleration * linearDistanceToDestination),
                              directionControl.maxSpeed);
    }
    else
    {
        idealSpeed = 0.0f;
    }

    auto idealLocalVelocity = localDirectionToDestination * idealSpeed;

    auto idealLocalVelocityCorrection = idealLocalVelocity - localLinearVelocity;
    auto idealLocalSpeedCorrection = glm::length(idealLocalVelocityCorrection);
    auto idealLocalVelocityCorrectionControl = m_flightControl->config().direction(idealLocalVelocityCorrection);

    if (EpsilonGt(glm::length(idealLocalVelocityCorrection), 0.0f))
    {
        const auto feasibleCorrection = idealLocalVelocityCorrectionControl.acceleration * seconds;
        const auto appliedCorrectionFactor = std::max(feasibleCorrection / idealLocalSpeedCorrection, 1.0f);

        const auto appliedAcceleration = glm::normalize(idealLocalVelocityCorrection) * appliedCorrectionFactor *
            idealLocalVelocityCorrectionControl.acceleration;

        m_flightControl->setLocalLinearAcceleration(appliedAcceleration);
    }
    else
    {
        m_flightControl->setLocalLinearAcceleration(glm::vec3());
    }
}