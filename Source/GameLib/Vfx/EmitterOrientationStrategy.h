#pragma once

#include <glm/glm.hpp>

#include "VfxParticle.h"

class EmitterOrientationStrategy
{
public:
    virtual ~EmitterOrientationStrategy() = default;
    virtual glm::quat                  generateOrientation() const;
    virtual VfxParticleOrientationType orientationType() const = 0;
};

class EmitterViewBillboardStrategy : public EmitterOrientationStrategy
{
public:
    VfxParticleOrientationType orientationType() const override;
};