#pragma once

inline VfxParticleId::VfxParticleId(size_t index, size_t renderBatchIndex)
    : index(index), renderBatchIndex(renderBatchIndex)
{
}

inline VfxParticle::VfxParticle(
    VfxMeshId         meshId,
    const glm::vec3 & origin,
    const glm::vec3 & velocity,
    TimestampMillis   birth,
    DurationMillis    lifetime)
    : meshId(meshId)
    , origin(origin)
    , velocity(velocity)
    , birth(birth)
    , lifetime(lifetime)
{
}