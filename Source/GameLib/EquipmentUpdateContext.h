#pragma once

#include <memory>

#include <Deliberation/Core/Math/Pose3D.h>
#include <Deliberation/ECS/Entity.h>

#include "GameLib.h"

namespace deliberation {
class RigidBody;
}

struct EquipmentUpdateContext
{
    Pose3D                      targetPose;
    std::shared_ptr<RigidBody> body;
    Entity    entity;
};
