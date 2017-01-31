#include "EmitterPlacementStrategy.h"

#include <Deliberation/Core/Math/Random.h>

EmitterGaussianSphericalPlacement::EmitterGaussianSphericalPlacement(float radius, float standardDeviation):
    m_dist(radius, standardDeviation)
{

}

glm::vec3 EmitterGaussianSphericalPlacement::generatePosition() const
{
    return RandomInSphere() * m_dist(m_engine);
}