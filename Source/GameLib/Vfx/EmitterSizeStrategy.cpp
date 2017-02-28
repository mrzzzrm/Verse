#include "EmitterSizeStrategy.h"

EmitterSizeOverLifetime::EmitterSizeOverLifetime(float scale):
    m_birthScale(scale), m_deathScale(scale)
{

}

EmitterSizeOverLifetime::EmitterSizeOverLifetime(float birthScale, float deathScale):
    m_birthScale(birthScale), m_deathScale(deathScale)
{

}

EmitterSizeStrategy::Result EmitterSizeOverLifetime::generate() const
{
    return {m_birthScale, m_deathScale};
}