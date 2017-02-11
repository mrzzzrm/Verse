#include "NpcAttackTask.h"

#include <iostream>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/FloatUtils.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Random.h>

#include "NpcController.h"

NpcAttackTask::NpcAttackTask()
{

}

void NpcAttackTask::setTarget(Entity target)
{
    m_target = target;
}

void NpcAttackTask::update(NpcController & controller, float seconds)
{
    if (!m_target.isValid()) return;

    auto & steering = controller.steering();
    const auto & body = controller.body();
    const auto & targetBody = m_target.component<std::shared_ptr<RigidBody>>();

    const auto targetPosition = targetBody->transform().position();
    const auto deltaToTarget = targetPosition - body->transform().position();

    const auto distanceToTarget = glm::length(deltaToTarget);

    if (m_status == Status::None)
    {
        if (distanceToTarget < 400.0f) m_status = Status::Evade;
        else m_status = Status::Joust;
    }

    std::cout << "Status: " << (int)m_status << std::endl;

    if (m_status == Status::Evade)
    {
        const auto targetFlyDirection = targetBody->linearVelocity();
        const auto targetDirection = EpsilonGt(glm::length2(targetFlyDirection), 0.0f) ?
                                     glm::normalize(targetFlyDirection) : glm::vec3(0, 0, 1);

        glm::vec3 evasionPoint;

        if (glm::dot(deltaToTarget, targetDirection) > 0.0f)
        {
            evasionPoint = targetPosition - targetDirection * 750.0f;
        }
        else
        {
            const auto planarEvasionPoint = RandomOnCircle() * 750.0f;

            auto planeX = AnyPerpendicularVectorTo(targetDirection);
            auto planeY = glm::cross(targetDirection, planeX);

            evasionPoint = planeX * planarEvasionPoint.x + planeY * planarEvasionPoint.y;
        }

        steering.setDestination(evasionPoint);

        if (distanceToTarget > 600.0f) m_status = Status::Joust;
    }
    else if (m_status == Status::Joust)
    {
        steering.setDestination(targetPosition);

        if (distanceToTarget < 300.0f) m_status = Status::Evade;
    }

    steering.setStopAtDestination(false);
}