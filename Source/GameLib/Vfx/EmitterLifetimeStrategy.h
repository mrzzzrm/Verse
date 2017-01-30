#pragma once

#include <random>

class EmitterLifetimeStrategy
{
public:
    virtual ~EmitterLifetimeStrategy() = default;
    virtual float generateLifetime() const = 0;
};

class EmitterRandomLifetime final:
    public EmitterLifetimeStrategy
{
public:
    EmitterRandomLifetime(float min, float max);

    float generateLifetime() const override;

private:
    mutable std::default_random_engine  m_engine;
    mutable std::uniform_real_distribution<float>
                                        m_dist;
};