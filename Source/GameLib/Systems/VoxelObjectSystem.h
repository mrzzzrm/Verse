#pragma once

#include <Deliberation/ECS/System.h>

#include "GameLib.h"

namespace deliberation
{
    class PhysicsWorld;
}

class VoxelWorld;

class VoxelObjectSystem:
    public System<VoxelObjectSystem>
{
public:
    VoxelObjectSystem(World & world,
                      PhysicsWorld & physicsWorld,
                      VoxelWorld & voxelWorld);

protected:
    void onEntityAdded(Entity & entity) override;
    void onEntityRemoved(Entity & entity) override;
    void onUpdate(Entity & entity, float seconds) override;

private:
    VoxelWorld &    m_voxelWorld;
    PhysicsWorld &  m_physicsWorld;
};