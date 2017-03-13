#include "RigidBodyPrototype.h"

#include <Deliberation/ECS/Components.h>

#include <Deliberation/Physics/RigidBody.h>

#include "VoxelRigidBodyPayload.h"

RigidBodyPrototype::RigidBodyPrototype(const Json & json)
{

}

void RigidBodyPrototype::applyToEntity(Entity & entity) const
{
    auto & voxelObject = entity.component<VoxelObject>();

    auto rigidBodyPayload = std::make_shared<VoxelRigidBodyPayload>(voxelObject.shared_from_this());

    auto rigidBody = std::make_shared<RigidBody>(voxelObject.data().shape());
    rigidBody->setPayload(rigidBodyPayload);
//    rigidBody->transform().setScale(voxelObject.pose().scale());
    rigidBody->transform().setCenter(glm::vec3(voxelObject.data().size()) / 2.0f);

    entity.addComponent<RigidBodyComponent>(rigidBody);
}