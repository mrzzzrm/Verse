#pragma once

#include <Deliberation/ECS/Defines.h>

#include "GameLib.h"
#include "VfxParticle.h"

struct HailstormBulletId
{
    VfxParticleId particleId;
    size_t physicsIndex = 0;
};

struct HailstormBullet
{
    inline HailstormBullet(VfxParticle particle, float intensity, float impactRadius, entity_id_t creator);

    VfxParticle         particle;
    HailstormBulletId   id;
    float               intensity = 0.0f;
    float               impactRadius = 0.0f;
    entity_id_t         creator = ECS_INVALID_ENTITY_ID;
};

#include "HailstormBullet.inl"