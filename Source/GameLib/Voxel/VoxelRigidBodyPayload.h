#pragma once

#include <memory>

#include <Deliberation/Physics/RigidBody.h>

#include "GameLib.h"
#include "VoxelObject.h"

struct VoxelRigidBodyPayload:
    public RigidBodyPayload
{
    VoxelRigidBodyPayload(std::weak_ptr<VoxelObject> object);

    std::weak_ptr<VoxelObject> object;
};