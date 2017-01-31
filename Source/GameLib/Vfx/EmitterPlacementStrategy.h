#pragma once

#include <random>

#include <glm/glm.hpp>

#include "GameLib.h"

class EmitterPlacementStrategy
{
public:
    virtual ~EmitterPlacementStrategy() = default;
    virtual glm::vec3 generatePosition() const = 0;
};

class EmitterFixedPlacement:
    public EmitterPlacementStrategy
{
public:
    glm::vec3 generatePosition() const override { return glm::vec3(0.0f); }
};

class EmitterGaussianSphericalPlacement:
    public EmitterPlacementStrategy
{
public:
    EmitterGaussianSphericalPlacement(float radius, float standardDeviation);

    glm::vec3 generatePosition() const override;

private:
    mutable std::default_random_engine  m_engine;
    mutable std::normal_distribution<float>
                                        m_dist;
};