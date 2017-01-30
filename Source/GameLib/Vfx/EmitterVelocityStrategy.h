#pragma once

#include <glm/glm.hpp>

#include <random>

class EmitterVelocityStrategy
{
public:
    virtual ~EmitterVelocityStrategy() = default;
    virtual glm::vec3 generateVelocity() const = 0;
};

class EmitterConeStrategy final:
    public EmitterVelocityStrategy
{
public:
    EmitterConeStrategy(float angle, float minSpeed, float maxSpeed);

    glm::vec3 generateVelocity() const override;

private:
    float                       m_angle;
    float                       m_minSpeed;
    float                       m_maxSpeed;
    mutable std::default_random_engine
                                m_engine;
    mutable std::uniform_real_distribution<float>
                                m_dist;
};