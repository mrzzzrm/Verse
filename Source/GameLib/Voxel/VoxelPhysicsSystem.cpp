#include "VoxelPhysicsSystem.h"

#include <Deliberation/ECS/RigidBodyComponent.h>

#include "VoxelObject.h"
#include "VoxelRigidBodyPayload.h"
#include "VoxelRigidBodyPrototype.h"

VoxelPhysicsSystem::VoxelPhysicsSystem(World & world)
    : Base(
          world,
          ComponentFilter::requires<VoxelObject, VoxelRigidBodyComponent>())
{
}

void VoxelPhysicsSystem::onEntityAdded(Entity & entity)
{
    auto & voxelObject = entity.component<VoxelObject>();
    auto rigidBody = std::make_shared<RigidBody>(voxelObject.data()->shape());
    entity.addComponent<RigidBodyComponent>(rigidBody);
}