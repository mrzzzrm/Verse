#include "EmitterIntensityStrategy.h"

EmitterNoisyIntensity::EmitterNoisyIntensity(float frequency, float standardDeviation):
    m_dist(1.0f / frequency, (1.0f / frequency) * standardDeviation)
{

}

float EmitterNoisyIntensity::generateInterval() const
{
    return std::max(0.0f, m_dist(m_engine));
}