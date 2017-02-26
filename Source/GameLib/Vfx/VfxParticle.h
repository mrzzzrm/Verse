#pragma once

#include <limits>

#include <glm/glm.hpp>

#include <Deliberation/Core/Chrono.h>

#include "GameLib.h"
#include "VfxDefines.h"
#include "VoxelDefines.h"

struct VfxParticleId
{
    VfxParticleId() = default;
    VfxParticleId(size_t index, size_t renderBatchIndex);

    size_t index = 0;
    size_t renderBatchIndex = 0;
};

struct VfxParticle
{
    VfxParticle(
        VfxMeshId meshID,
        const glm::vec3 & origin,
        const glm::vec3 & velocity,
        TimestampMillis birth,
        DurationMillis lifetime);

    VfxMeshId           meshId;
    glm::vec3           origin;
    glm::vec3           velocity;
    DurationMillis      lifetime = 0;
    TimestampMillis     birth = 0;
};

#include "VfxParticle.inl"