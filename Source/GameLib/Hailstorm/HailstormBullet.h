#pragma once

#include <memory>

#include <Deliberation/ECS/Defines.h>

#include "Emitter.h"
#include "GameLib.h"
#include "VfxParticle.h"

struct HailstormBulletId
{
    VfxParticleId particleId;
    size_t        physicsIndex = 0;
};

struct HailstormBullet
{
    inline HailstormBullet(
        VfxParticle particle,
        float       intensity,
        float       impactRadius,
        EntityId    creator);

    VfxParticle       particle;
    HailstormBulletId id;
    float             intensity = 0.0f;
    float             impactRadius = 0.0f;
    EntityId          creator = ECS_INVALID_ENTITY_ID;
    std::shared_ptr<const Emitter> explosionEmitter;
};

#include "HailstormBullet.inl"