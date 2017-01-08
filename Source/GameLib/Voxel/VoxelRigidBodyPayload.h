#pragma once

#include <Deliberation/Physics/RigidBody.h>

#include "GameLib.h"
#include "VoxelObject.h"

struct VoxelRigidBodyPayload:
    public RigidBodyPayload
{
    VoxelRigidBodyPayload(VoxelObjectID voxelObjectID);

    VoxelObjectID voxelObjectID;
};