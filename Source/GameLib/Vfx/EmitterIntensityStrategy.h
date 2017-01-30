#pragma once

#include <random>

class EmitterIntensityStrategy
{
public:
    virtual ~EmitterIntensityStrategy() = default;
    virtual float generateInterval() const = 0;
};

class EmitterNoisyIntensity:
    public EmitterIntensityStrategy
{
public:
    EmitterNoisyIntensity(float frequency, float standardDeviation);

    float generateInterval() const override;

private:
    mutable std::default_random_engine  m_engine;
    mutable std::normal_distribution<float>
                                m_dist;
};