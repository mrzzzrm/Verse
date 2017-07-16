#include "EmitterLifetimeStrategy.h"

#include <Deliberation/Core/Assert.h>

EmitterRandomLifetime::EmitterRandomLifetime(float min, float max)
    : m_dist(min, max)
{
    Assert(max >= min, "");
}

float EmitterRandomLifetime::generateLifetime() const
{
    return m_dist(m_engine);
}