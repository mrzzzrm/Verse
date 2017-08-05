#pragma once

#include <memory>
#include <random>

#include "GameLib.h"

class EmitterInstance;
class EmitterInstanceContext;

class EmitterIntensityContext
{
public:
    virtual ~EmitterIntensityContext() = default;
};

class EmitterIntensityStrategy
{
public:
    static constexpr float NO_FURTHER_EMISSIONS =
        std::numeric_limits<float>::max();

public:
    virtual ~EmitterIntensityStrategy() = default;
    virtual float generateInterval(EmitterInstance & instance, EmitterInstanceContext & context) const = 0;
    virtual std::shared_ptr<EmitterIntensityContext> createContext() const;
};

class EmitterFixedIntensity : public EmitterIntensityStrategy
{
public:
    explicit EmitterFixedIntensity(float frequency):
        m_frequency(frequency)
    {}

    float generateInterval(EmitterInstance & instance, EmitterInstanceContext & context) const override
    {
        return 1.0f / m_frequency;
    }

private:
    float m_frequency;
};

class EmitterNoisyIntensity : public EmitterIntensityStrategy
{
public:
    EmitterNoisyIntensity(float frequency, float standardDeviation);

    float generateInterval(EmitterInstance & instance, EmitterInstanceContext & context) const override;

private:
    mutable std::default_random_engine      m_engine;
    mutable std::normal_distribution<float> m_dist;
};

class EmitterBurstIntensity : public EmitterIntensityStrategy
{
public:
    struct Context : public EmitterIntensityContext
    {
        u32 countdown = 0;
    };

public:
    EmitterBurstIntensity(float mean, float standardDeviation);

    float generateInterval(EmitterInstance & instance, EmitterInstanceContext & context) const override;

    std::shared_ptr<EmitterIntensityContext> createContext() const override;

private:
    mutable std::default_random_engine      m_engine;
    mutable std::normal_distribution<float> m_dist;
};
