#pragma once

#include <limits>

#include <boost/optional.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Deliberation/Core/Chrono.h>
#include <Deliberation/Core/IntTypes.h>

#include <Deliberation/Scene/Lighting/PointLightRenderer.h>

#include "GameLib.h"
#include "VfxDefines.h"
#include "VfxPointLight.h"
#include "VoxelDefines.h"

// TODO compress this
struct VfxParticleId
{
    static constexpr size_t INVALID_UID = std::numeric_limits<size_t>::max();

    static size_t uidCounter;

    size_t uid = INVALID_SIZE_T;
    size_t meshRenderBatchSlot = INVALID_SIZE_T;
    size_t meshRenderBatchIndex = INVALID_SIZE_T;
    size_t particlePointLight = INVALID_SIZE_T;

    VfxParticleId(): uid(uidCounter++) {}

    friend std::ostream & operator<<(std::ostream & o, const VfxParticleId & particleId) {
        o << "{" << particleId.uid << " | " << particleId.meshRenderBatchSlot << ", "
          << particleId.meshRenderBatchIndex << ", ";

        if (particleId.particlePointLight == INVALID_SIZE_T) o << "no pointlight";
        else o << particleId.particlePointLight;

        o << "}";

        return o;
    }
};

struct VfxParticle
{
    static constexpr DurationMillis INFINITE_LIFETIME = std::numeric_limits<u32>::max(); // Shader uses 32 bit uint

    VfxParticle(
        const glm::vec3 & origin,
        const glm::vec3 & velocity,
        TimestampMillis   birth,
        DurationMillis    lifetime);

    size_t          meshRenderBatchIndex = INVALID_VFX_MESH_RENDER_BATCH_INDEX;
    glm::vec3       origin;
    glm::vec3       velocity;
    DurationMillis  lifetime = 0;
    TimestampMillis birth = 0;

    glm::vec4 birthRGBA = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
    glm::vec4 deathRGBA = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);

    float birthScale = 1.0f;
    float deathScale = 1.0f;

    glm::quat birthOrientation;
    glm::vec4 axisRotation;

    boost::optional<VfxPointLightDesc> pointLight;
};

#include "VfxParticle.inl"