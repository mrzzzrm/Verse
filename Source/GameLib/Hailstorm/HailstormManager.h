#pragma once

#include "GameLib.h"
#include "HailstormBullet.h"
#include "HailstormPhysicsWorld.h"
#include "VfxManager.h"

namespace deliberation
{
class PhysicsWorld;
}

class HailstormManager final
{
public:
    HailstormManager(
        Context & context,
        const Camera3D & camera,
        PhysicsWorld & physicsWorld,
        VoxelWorld & voxelWorld);

    void addBullet(HailstormBullet bullet);

    void update(float seconds);

    void render();

private:
    VfxManager              m_vfxManager;
    HailstormPhysicsWorld   m_hailstormPhysicsWorld;
};
