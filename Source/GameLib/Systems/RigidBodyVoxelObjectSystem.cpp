#include "RigidBodyVoxelObjectSystem.h"

#include <Deliberation/Core/Math/Pose3D.h>
#include <Deliberation/Physics/RigidBody.h>

#include "VoxelObject.h"

RigidBodyVoxelObjectSystem::RigidBodyVoxelObjectSystem(World & world):
    Base(world, ComponentFilter::requires<std::shared_ptr<RigidBody>, std::shared_ptr<VoxelObject>>())
{

}

void RigidBodyVoxelObjectSystem::onUpdate(Entity & entity, float seconds)
{
    auto & body = entity.component<std::shared_ptr<RigidBody>>();
    auto & object = entity.component<std::shared_ptr<VoxelObject>>();

    Pose3D pose;
    pose.setPosition(body->transform().position());
    pose.setOrientation(body->transform().orientation());
    pose.setCenter(body->transform().center());

    object->setPose(pose);
}