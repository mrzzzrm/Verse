#include "EmitterRotationStrategy.h"

#include <Deliberation/Core/Math/Random.h>

glm::vec4 EmitterBillboardRotation::generateRotation()
{
    return glm::vec4(0.0f, 0.0f, 1.0f, RandomFloat(m_minAngularSpeed, m_maxAngularSpeed));
}