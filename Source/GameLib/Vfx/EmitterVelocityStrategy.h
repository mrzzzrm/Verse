#pragma once

#include <random>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Random.h>

#include "GameLib.h"

class EmitterVelocityStrategy
{
public:
    virtual ~EmitterVelocityStrategy() = default;
    virtual glm::vec3 generateVelocity() const = 0;
};

class EmitterConeStrategy final : public EmitterVelocityStrategy
{
public:
    EmitterConeStrategy(float angle, float minSpeed, float maxSpeed);

    glm::vec3 generateVelocity() const override;

private:
    float                                         m_angle;
    float                                         m_minSpeed;
    float                                         m_maxSpeed;
    mutable std::default_random_engine            m_engine;
    mutable std::uniform_real_distribution<float> m_dist;
};

class EmitterAnyDirection final : public EmitterVelocityStrategy
{
public:
    EmitterAnyDirection(float minSpeed, float maxSpeed);

    glm::vec3 generateVelocity() const override;

private:
    float m_minSpeed;
    float m_maxSpeed;
};

class EmitterFixedDirection final : public EmitterVelocityStrategy
{
public:
    EmitterFixedDirection(float minSpeed, float maxSpeed);

    glm::vec3 generateVelocity() const override;

private:
    float m_minSpeed;
    float m_maxSpeed;
};