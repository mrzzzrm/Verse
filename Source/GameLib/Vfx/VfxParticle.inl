#pragma once

inline VfxParticle::VfxParticle(
    const glm::vec3 & origin,
    const glm::vec3 & velocity,
    TimestampMillis   birth,
    DurationMillis    lifetime):
     origin(origin)
    , velocity(velocity)
    , birth(birth)
    , lifetime(lifetime)
{
}