#include "VoxelObjectSystem.h"

#include <Deliberation/Core/Math/Pose3D.h>
#include <Deliberation/Physics/RigidBody.h>
#include <Deliberation/Physics/PhysicsWorld.h>

#include "VoxelObject.h"
#include "VoxelWorld.h"

VoxelObjectSystem::VoxelObjectSystem(World & world,
                                     PhysicsWorld & physicsWorld,
                                     VoxelWorld & voxelWorld):
    Base(world, ComponentFilter::requires<std::shared_ptr<VoxelObject>, std::shared_ptr<RigidBody>>()),
    m_voxelWorld(voxelWorld),
    m_physicsWorld(physicsWorld)
{

}

void VoxelObjectSystem::onEntityAdded(Entity & entity)
{
    auto & voxelObject = entity.component<std::shared_ptr<VoxelObject>>();
    m_voxelWorld.addVoxelObject(voxelObject);
}

void VoxelObjectSystem::onEntityRemoved(Entity & entity)
{
    auto & voxelObject = entity.component<std::shared_ptr<VoxelObject>>();
    //m_voxelWorld.removeVoxelObject(voxelObject);
}

void VoxelObjectSystem::onUpdate(Entity & entity, float seconds)
{
    if (entity.hasComponent<std::shared_ptr<RigidBody>>())
    {
        auto & body = entity.component<std::shared_ptr<RigidBody>>();
        auto & object = entity.component<std::shared_ptr<VoxelObject>>();

        Pose3D pose;
        pose.setPosition(body->transform().position());
        pose.setOrientation(body->transform().orientation());
        pose.setCenter(body->transform().center());

        object->setPose(pose);
    }
}