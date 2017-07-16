#include "EmitterPlacementStrategy.h"

#include <Deliberation/Core/Math/Random.h>

EmitterGaussianSphericalPlacement::EmitterGaussianSphericalPlacement(
    float radius, float standardDeviation)
    : m_dist(radius, standardDeviation)
{
}

glm::vec3 EmitterGaussianSphericalPlacement::generatePosition() const
{
    return RandomInSphere() * m_dist(m_engine);
}

EmitterGaussianCircularPlacement::EmitterGaussianCircularPlacement(
    float radius, float standardDeviation)
    : m_dist(radius, standardDeviation)
{
}

glm::vec3 EmitterGaussianCircularPlacement::generatePosition() const
{
    return glm::vec3(RandomOnCircle(), 0.0f) * m_dist(m_engine);
}