#include "NpcDebugTask.h"

#include <iostream>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/FloatUtils.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Random.h>

#include <Deliberation/ECS/Components.h>

#include "NpcController.h"

NpcDebugTask::NpcDebugTask() {}

void NpcDebugTask::setFireRequest(bool enabled, const glm::vec3 & direction)
{
    m_fireRequestEnabled = enabled;
    m_fireRequestDirection = direction;
}

void NpcDebugTask::update(
    NpcController & controller,
    RigidBody &     body,
    Equipment &     equipment,
    const UpdateFrame & updateFrame)
{
    if (m_fireRequestEnabled)
        equipment.setFireRequestDirectionForAllHardpoints(
            m_fireRequestDirection);
    else
        equipment.clearFireRequests();
}