#pragma once

#include <memory>
#include <random>

#include "GameLib.h"

class EmitterInstance;

class EmitterIntensityContext
{
public:
    virtual ~EmitterIntensityContext() = default;
};

class EmitterIntensityStrategy
{
public:
    static constexpr float NO_FURTHER_EMISSIONS = std::numeric_limits<float>::max();

public:
    virtual ~EmitterIntensityStrategy() = default;
    virtual float generateInterval(EmitterInstance & instance) const = 0;
    virtual std::shared_ptr<EmitterIntensityContext> createContext() const;
};

class EmitterNoisyIntensity:
    public EmitterIntensityStrategy
{
public:
    EmitterNoisyIntensity(float frequency, float standardDeviation);

    float generateInterval(EmitterInstance & instance) const override;

private:
    mutable std::default_random_engine  m_engine;
    mutable std::normal_distribution<float>
                                m_dist;
};

class EmitterBurstIntensity:
    public EmitterIntensityStrategy
{
public:
    struct Context:
        public EmitterIntensityContext
    {
        u32 countdown = 0;
    };

public:
    EmitterBurstIntensity(float mean, float standardDeviation);

    float generateInterval(EmitterInstance & instance) const override;

    std::shared_ptr<EmitterIntensityContext> createContext() const override;

private:
    mutable std::default_random_engine      m_engine;
    mutable std::normal_distribution<float> m_dist;
};
