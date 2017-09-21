#include "NpcAttackTask.h"

#include <iostream>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/FloatUtils.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Random.h>
#include <Deliberation/Core/Math/Trajectory.h>

#include <Deliberation/ECS/RigidBodyComponent.h>

#include "NpcController.h"
#include "Weapon.h"

NpcAttackTask::NpcAttackTask() {}

const Entity & NpcAttackTask::target() const { return m_target; }

void NpcAttackTask::setTarget(Entity target) { m_target = target; }

void NpcAttackTask::update(
    NpcController & controller,
    RigidBody &     body,
    Equipment &     equipment,
    const UpdateFrame & updateFrame)
{
    if (!m_target.isValid())
    {
        equipment.clearFireRequests();
        return;
    }

    auto & steering = controller.steering();

    const auto & targetBody = m_target.component<RigidBodyComponent>().value();

    const auto targetPosition = targetBody->transform().position();
    const auto deltaToTarget = targetPosition - body.transform().position();

    const auto flyDirection =
        EpsilonGt(glm::length2(body.linearVelocity()), 0.0f)
            ? glm::normalize(body.linearVelocity())
            : glm::vec3();
    const auto targetFlyDirection = targetBody->linearVelocity();
    const auto targetDirection =
        EpsilonGt(glm::length2(targetFlyDirection), 0.0f)
            ? glm::normalize(targetFlyDirection)
            : glm::vec3(0, 0, 1);
    const auto distanceToTarget = glm::length(deltaToTarget);
    const auto directionToTarget = glm::normalize(deltaToTarget);

    if (m_status == Status::None)
    {
        if (distanceToTarget < 800.0f)
            startEvasion(body, controller);
        else
            startJoust();
    }

    if (m_status == Status::Evade)
    {
        steering.setDestination(m_evasionPoint);

        auto distanceToEvasionPoint =
            glm::length(m_evasionPoint - body.transform().position());

        if (distanceToEvasionPoint < 100.0f)
            startEvasion(body, controller);
        else if (distanceToTarget > 700.0f)
            startJoust();
        //  else if (glm::dot(directionToTarget, flyDirection) < 0.0f &&
        //  distanceToTarget > 200.0f) startJoust();
    }
    else if (m_status == Status::Joust)
    {
        steering.setDestination(targetPosition);

        if (distanceToTarget < 600.0f) startEvasion(body, controller);
    }

    steering.setStopAtDestination(false);

    /**
     * Setup fire request
     */
    auto bulletSpeed = equipment.bulletSpeed();

    if (bulletSpeed != 0.0f)
    {
        const auto & bodyPosition = body.transform().position();
        const auto & bodyVelocity = body.linearVelocity();
        const auto & targetVelocity = targetBody->linearVelocity();

        auto       success = false;
        const auto trajectory = CalculateTrajectory(
            bodyPosition,
            bodyVelocity,
            bulletSpeed,
            targetPosition,
            targetVelocity,
            success);

        if (success)
            equipment.setFireRequestDirectionForAllHardpoints(
                glm::normalize(trajectory));
        else
            equipment.clearFireRequests();
    }
}

void NpcAttackTask::startEvasion(RigidBody & body, NpcController & controller)
{
    const auto & targetBody = m_target.component<RigidBodyComponent>().value();

    const auto targetPosition = targetBody->transform().position();
    const auto deltaToTarget = targetPosition - body.transform().position();

    const auto targetFlyDirection = targetBody->linearVelocity();
    const auto targetDirection =
        EpsilonGt(glm::length2(targetFlyDirection), 0.0f)
            ? glm::normalize(targetFlyDirection)
            : glm::vec3(0, 0, 1);
    const auto distanceToTarget = glm::length(deltaToTarget);
    const auto directionToTarget = glm::normalize(deltaToTarget);

    //    if (glm::dot(deltaToTarget, targetDirection) > 0.0f)
    //    {
    //        std::cout << "  Evasion 1" << std::endl;
    //        m_evasionPoint = targetPosition - targetDirection * 250.0f;
    //    }
    //    else
    //    {
    //     std::cout << "  Evasion 2" << std::endl;
    const auto planarEvasionPoint = RandomOnCircle() * 200.0f;

    auto planeX = AnyPerpendicularVectorTo(directionToTarget);
    auto planeY = glm::cross(targetDirection, planeX);

    m_evasionPoint = targetPosition + planeX * planarEvasionPoint.x +
                     planeY * planarEvasionPoint.y + directionToTarget * 300.0f;
    //    }

    m_status = Status::Evade;
}

void NpcAttackTask::startJoust()
{
    //    std::cout << "  Joust" << std::endl;
    m_status = Status::Joust;
}