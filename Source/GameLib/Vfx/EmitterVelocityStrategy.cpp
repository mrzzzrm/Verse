#include "EmitterVelocityStrategy.h"

#include <glm/gtc/constants.hpp>

EmitterConeStrategy::EmitterConeStrategy(float angle, float minSpeed, float maxSpeed):
    m_angle(angle),
    m_minSpeed(minSpeed),
    m_maxSpeed(maxSpeed),
    m_dist(0.0f, 1.0f)
{

}

glm::vec3 EmitterConeStrategy::generateVelocity() const
{
    const auto radius = m_dist(m_engine);
    const auto angle = m_dist(m_engine) * 2.0f * glm::pi<float>();

    const auto direction = glm::vec3(std::cos(angle) * radius,
                                     std::sin(angle) * radius,
                                     -1.0f / std::tan(m_angle));

    const auto normalizedDirection = glm::normalize(direction);
    const auto speed = m_minSpeed + m_dist(m_engine) * (m_maxSpeed - m_minSpeed);

    const auto velocity = normalizedDirection * speed;

    return velocity;
}

EmitterAnyDirection::EmitterAnyDirection(float minSpeed, float maxSpeed):
    m_minSpeed(minSpeed),
    m_maxSpeed(maxSpeed)
{

}

glm::vec3 EmitterAnyDirection::generateVelocity() const
{
    return RandomUnitVec3() * RandomFloat(m_minSpeed, m_maxSpeed);
}

EmitterFixedDirection::EmitterFixedDirection(float minSpeed, float maxSpeed):
    m_minSpeed(minSpeed),
    m_maxSpeed(maxSpeed)
{}

glm::vec3 EmitterFixedDirection::generateVelocity() const
{
    return glm::vec3(0, 0, -1) * RandomFloat(m_minSpeed, m_maxSpeed);
}