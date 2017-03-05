#include "NpcDebugTask.h"

#include <iostream>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/FloatUtils.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Random.h>

#include <Deliberation/ECS/Components.h>

#include "NpcController.h"


NpcDebugTask::NpcDebugTask()
{

}

void NpcDebugTask::setFireRequest(bool enabled, const glm::vec3 & target)
{
    m_fireRequestEnabled = enabled;
    m_fireRequestTarget = target;
}

void NpcDebugTask::update(NpcController & controller, Equipment & equipment, float seconds)
{
    equipment.setFireRequest(m_fireRequestEnabled, m_fireRequestTarget);
}