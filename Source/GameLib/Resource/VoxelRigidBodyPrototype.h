#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Core/Json.h>
#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/ECS/Component.h>
#include <Deliberation/ECS/ComponentPrototype.h>
#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/World.h>

#include "GameLib.h"
#include "VoxelWorld.h"

class VoxelRigidBodyComponent : public Component<VoxelRigidBodyComponent>
{
    DELIBERATION_COMPONENT_NAME("VoxelRigidBody")
};

class VoxelRigidBodyPrototype final
    : public ComponentPrototype<VoxelRigidBodyComponent>
{
public:
    void updateComponent(const Entity & entity, VoxelRigidBodyComponent & rigidBodyComponent) override
    {
    }
};