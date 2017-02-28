#pragma once

#include <random>

#include "GameLib.h"

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

class EmitterBurstIntensity:
    public EmitterIntensityStrategy
{
public:
    EmitterBurstIntensity(float mean, float standardDeviation);

    float generateInterval() const override;

private:
    mutable u32 m_countdown = 0;
};

class EmitterFalloffIntensity:
    public EmitterIntensityStrategy
{
public:
    EmitterFalloffIntensity(float intercept, float standardDeviation);

    float generateInterval() const override;

private:
    mutable std::default_random_engine  m_engine;
    mutable std::normal_distribution<float>
                                m_dist;
};