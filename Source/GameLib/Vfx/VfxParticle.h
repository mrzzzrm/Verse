#pragma once

#include <limits>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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
        const glm::vec3 & origin,
        const glm::vec3 & velocity,
        TimestampMillis   birth,
        DurationMillis    lifetime);

    size_t          renderBatchIndex = INVALID_VFX_RENDER_BATCH_INDEX;
    glm::vec3       origin;
    glm::vec3       velocity;
    DurationMillis  lifetime = 0;
    TimestampMillis birth = 0;

    glm::vec4 birthRGBA = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
    glm::vec4 deathRGBA = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);

    float birthScale = 1.0f;
    float deathScale = 1.0f;

    glm::quat birthOrientation;
};

#include "VfxParticle.inl"