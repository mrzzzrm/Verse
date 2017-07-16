#include "EmitterRotationStrategy.h"

glm::quat EmitterRotationStrategy::generateOrientation() const { return {}; }

VfxParticleOrientationType EmitterViewBillboardStrategy::orientationType() const
{
    return VfxParticleOrientationType::ViewBillboard;
}