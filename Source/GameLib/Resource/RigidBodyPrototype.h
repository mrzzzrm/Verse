#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Core/Json.h>
#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/ECS/World.h>

#include "ComponentPrototype.h"
#include "GameLib.h"
#include "VoxelWorld.h"

class RigidBodyPrototype final:
    public ComponentPrototype
{
public:
    RigidBodyPrototype(const nlohmann::json & json);

    void applyToEntity(Entity & entity) const override;
};