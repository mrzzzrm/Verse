#pragma once

inline VfxParticleId::VfxParticleId(size_t index, size_t renderBatchIndex)
    : index(index), renderBatchIndex(renderBatchIndex)
{
}

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