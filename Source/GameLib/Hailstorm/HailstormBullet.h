#pragma once

#include "GameLib.h"
#include "VfxParticle.h"

struct HailstormBulletId
{
    VfxParticleId particleId;
    size_t physicsIndex = 0;
};

struct HailstormBullet
{
    inline HailstormBullet(VfxParticle particle, float intensity, float impactRadius);

    VfxParticle particle;
    HailstormBulletId id;
    float intensity = 0.0f;
    float impactRadius = 0.0f;
};

#include "HailstormBullet.inl"