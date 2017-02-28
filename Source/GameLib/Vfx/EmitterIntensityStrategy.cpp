#include "EmitterIntensityStrategy.h"

EmitterNoisyIntensity::EmitterNoisyIntensity(float frequency, float standardDeviation):
    m_dist(1.0f / frequency, (1.0f / frequency) * standardDeviation)
{

}

float EmitterNoisyIntensity::generateInterval() const
{
    return std::max(0.0f, m_dist(m_engine));
}

EmitterBurstIntensity::EmitterBurstIntensity(float mean, float standardDeviation)
{
    std::default_random_engine engine;
    std::normal_distribution<float> dist(mean, standardDeviation);

    m_countdown = (u32)dist(engine);
}

float EmitterBurstIntensity::generateInterval() const
{
    if (m_countdown > 0)
    {
        m_countdown--;
        return 0.0f;
    }
    else
    {
        return std::numeric_limits<float>::max();
    }
}