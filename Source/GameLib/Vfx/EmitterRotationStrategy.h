#pragma once

#include <glm/glm.hpp>

#include "VfxParticle.h"

class EmitterRotationStrategy
{
public:
    virtual ~EmitterRotationStrategy() = default;
    virtual glm::quat generateOrientation() const;
    virtual VfxParticleOrientationType orientationType() const = 0;
};

class EmitterViewBillboardStrategy:
    public EmitterRotationStrategy
{
public:
    VfxParticleOrientationType orientationType() const override;
};