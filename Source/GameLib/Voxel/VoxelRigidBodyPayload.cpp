#include "VoxelRigidBodyPayload.h"

VoxelRigidBodyPayload::VoxelRigidBodyPayload(std::weak_ptr<VoxelObject> object)
    : object(object)
{
}