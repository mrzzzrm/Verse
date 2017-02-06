#include "FlyToTask.h"

#include <algorithm>
#include <iostream>

#include <glm/gtx/vector_angle.hpp>

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
    auto linearDistanceToDestination = glm::length(linearDeltaToDestination);

    if (linearDeltaToDestination == glm::vec3(0.0f))
    {
        m_flightControl->setLinearThrust(glm::vec3());
        m_flightControl->setAngularThrust(glm::vec3());
        return;
    }

    auto directionToDestination = glm::normalize(linearDeltaToDestination);
    auto localDirectionToDestination = predictedPose.directionWorldToLocal(directionToDestination);

//    /**
//     * Angular
//     */
//    auto angularDeltaToDestination = glm::angle(directionToDestination,
//                                                predictedPose.orientation() * glm::vec3(0.0f, 0.0f, -1.0f));
//
//
//    glm::vec3 idealRotationAxis;
//
//    if (std::abs(angularDeltaToDestination - glm::pi<float>()) < 0.05)
//    {
//        // any axis will do
//        idealRotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
//    }
//    else
//    {
//        idealRotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, -1.0f), localDirectionToDestination);
//    }
//
//    auto angleToDestination = std::asin(glm::length(idealRotationAxis));
//
////    if (angleToDestination < 0.1f)
////    {
////        auto destination = RandomInHemisphere({0.0f, 300.0f, 0.0f});
////        setDestination(destination);
////    }
//
//    auto idealAngularSpeed =
//        std::min(std::sqrt(2 * m_flightControl->config().angular.acceleration * angleToDestination),
//                 m_flightControl->config().angular.maxSpeed);
//
//    auto idealAngularVelocity = glm::normalize(idealRotationAxis) * idealAngularSpeed;
//
//    auto idealAngularVelocityCorrection = idealAngularVelocity - body->angularVelocity();
//
//    if (glm::length(idealAngularVelocityCorrection) > 0.001f)
//    {
//        m_flightControl->setAngularThrust(glm::normalize(idealAngularVelocityCorrection) * 0.3f);
//    }
//    else
//    {
//        m_flightControl->setAngularThrust(glm::vec3(0.0f));
//    }

    /**
     * Linear
     */
    auto idealFrame = m_flightControl->config().frame(localDirectionToDestination);

    float idealSpeed;
//    if (angleToDestination < 0.5f)
//    {
        idealSpeed = std::min(std::sqrt(2 * idealFrame.acceleration * linearDistanceToDestination),
                              idealFrame.maxSpeed);
//    }
//    else
//    {
//        idealSpeed = 0.0f;
//    }

    auto idealVelocity = directionToDestination * idealSpeed;

    auto idealVelocityCorrection = idealVelocity - body->linearVelocity();

    if (glm::length(idealVelocityCorrection) > 0.001f)
    {
        auto localIdealVelocityCorrection = predictedPose.directionWorldToLocal(idealVelocityCorrection);
        auto clampedLocalIdealAcceleration =
            m_flightControl->config().clampAcceleration(localIdealVelocityCorrection);

        std::cout << clampedLocalIdealAcceleration << std::endl;

        clampedLocalIdealAcceleration.x /= m_flightControl->config().horizontal.acceleration;
        clampedLocalIdealAcceleration.y /= m_flightControl->config().vertical.acceleration;
        clampedLocalIdealAcceleration.z /= clampedLocalIdealAcceleration.z <= 0.0f ? m_flightControl->config().forward.acceleration :
                                           m_flightControl->config().backward.acceleration;

        m_flightControl->setLinearThrust(clampedLocalIdealAcceleration);
    }
    else
    {
        m_flightControl->setLinearThrust(glm::vec3());
    }
}