#include "EmitterOrientationStrategy.h"

glm::quat EmitterOrientationStrategy::generateOrientation() const { return {}; }

VfxParticleOrientationType EmitterViewBillboardStrategy::orientationType() const
{
    return VfxParticleOrientationType::ViewBillboard;
}