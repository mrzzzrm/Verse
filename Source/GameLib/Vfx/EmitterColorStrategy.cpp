#include "EmitterColorStrategy.h"

EmitterConstColor::EmitterConstColor(const glm::vec4 & color):
    m_color(color)
{}

EmitterColorStrategy::Result EmitterConstColor::generate() const
{
    return {m_color, m_color};
}

EmitterColorOverLifetime::EmitterColorOverLifetime(const glm::vec4 & birthRGBA, const glm::vec4 & deathRGBA):
    m_birthRGBA(birthRGBA),
    m_deathRGBA(deathRGBA)
{}

EmitterColorStrategy::Result EmitterColorOverLifetime::generate() const
{
    return {m_birthRGBA, m_deathRGBA};
}