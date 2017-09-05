#include "VoxelPhysicsSystem.h"

#include <Deliberation/ECS/Components.h>

#include "VoxelObject.h"
#include "VoxelRigidBodyPayload.h"
#include "VoxelRigidBodyPrototype.h"

VoxelPhysicsSystem::VoxelPhysicsSystem(World & world)
    : Base(
          world,
          ComponentFilter::requires<VoxelObject, VoxelRigidBodyComponent, Transform3DComponent>())
{
}

void VoxelPhysicsSystem::onEntityAdded(Entity & entity)
{
    auto & voxelObject = entity.component<VoxelObject>();

    auto rigidBody = std::make_shared<RigidBody>(voxelObject.data()->shape());
    //    rigidBody->transform().setScale(voxelObject.localPose().scale());

    entity.component<Transform3DComponent>().value().setCenter(glm::vec3(voxelObject.data()->size()) / 2.0f);
    entity.addComponent<RigidBodyComponent>(rigidBody);

    entity.component<RigidBodyComponent>().value()->setTransform(
        entity.component<Transform3DComponent>().value());
}