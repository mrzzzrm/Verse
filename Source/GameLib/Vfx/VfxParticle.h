#pragma once

#include <limits>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Deliberation/Core/Chrono.h>

#include "GameLib.h"
#include "VfxDefines.h"
#include "VoxelDefines.h"

enum class VfxParticleOrientationType {
    World = 0,
    ViewBillboard = 1,

    _Count_
};

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

    glm::vec4           birthRGBA;
    glm::vec4           deathRGBA;

    float               birthScale = 1.0f;
    float               deathScale = 1.0f;

    VfxParticleOrientationType
                        orientationType = VfxParticleOrientationType::World;
    glm::quat           birthOrientation;
};

#include "VfxParticle.inl"