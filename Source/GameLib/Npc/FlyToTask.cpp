#include "FlyToTask.h"

#include <glm/gtx/vector_angle.hpp>

#include <Deliberation/Physics/RigidBody.h>

FlyToTask::FlyToTask(std::shared_ptr<FlightControl> flightControl, const glm::vec3 & destination):
    NpcTask(flightControl),
    m_destination(destination)
{

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

    m_flightControl->setAngularThrust(rotationAxis);
    m_flightControl->update(seconds);
    

}