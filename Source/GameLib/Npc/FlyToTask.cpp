#include "FlyToTask.h"

#include <iostream>

#include <glm/gtx/vector_angle.hpp>

#include <Deliberation/Core/Math/Random.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Physics/RigidBody.h>

FlyToTask::FlyToTask(std::shared_ptr<FlightControl> flightControl, const glm::vec3 & destination):
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
    auto deltaToDestination = m_destination - body->transform().position();
    auto directionToDestination = deltaToDestination == glm::vec3(0.0f) ?
                                  glm::vec3(0.0f) : glm::normalize(deltaToDestination);

    const auto currentDirection = body->transform().directionLocalToWorld(glm::vec3(0.0f, 0.0f, -1.0f));
    auto angleToDestination = glm::angle(directionToDestination, currentDirection);

    const auto rotationAxis = glm::cross(currentDirection, directionToDestination);

    m_flightControl->setAngularThrust(glm::normalize(rotationAxis) * 0.2f);

    if (angleToDestination < glm::pi<float>() / 10.0f)
    {
        auto destination = RandomInHemisphere({0.0f, 300.0f, 0.0f});
        setDestination(destination);
       // m_flightControl->setLinearThrust({0.0f, 0.0f, -1.0f});
    }
    else
    {
      //  m_flightControl->setLinearThrust({0.0f, 0.0f, 0.0f});
    }


}