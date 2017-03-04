#include "VoxelObjectSystem.h"

#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/ECS/Components.h>

#include <Deliberation/Physics/RigidBody.h>
#include <Deliberation/Physics/PhysicsWorld.h>

#include "Components.h"
#include "VoxelObject.h"
#include "VoxelWorld.h"

VoxelObjectSystem::VoxelObjectSystem(World & world,
                                     PhysicsWorld & physicsWorld,
                                     VoxelWorld & voxelWorld):
    Base(world, ComponentFilter::requires<VoxelObject, RigidBodyComponent>()),
    m_voxelWorld(voxelWorld),
    m_physicsWorld(physicsWorld)
{

}

void VoxelObjectSystem::onEntityAdded(Entity & entity)
{
    auto & voxelObject = entity.component<VoxelObject>();
    m_voxelWorld.addVoxelObject(voxelObject.shared_from_this());
}

void VoxelObjectSystem::onEntityRemoved(Entity & entity)
{
    auto & voxelObject = entity.component<VoxelObject>();
    //m_voxelWorld.removeVoxelObject(voxelObject);
}

void VoxelObjectSystem::onUpdate(Entity & entity, float seconds)
{
    if (entity.hasComponent<RigidBodyComponent>())
    {
        auto & body = entity.component<RigidBodyComponent>().value();
        auto & object = entity.component<VoxelObject>();

        Pose3D pose;
        pose.setPosition(body->transform().position());
        pose.setOrientation(body->transform().orientation());
        pose.setCenter(body->transform().center());

        object.setPose(pose);
    }
}