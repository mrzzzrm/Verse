#pragma once

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Physics/PhysicsWorld.h>

#include "Hailstorm/HailstormBullet.h"
#include "Hailstorm/HailstormRenderer.h"
#include "Hailstorm/HailstormPhysicsWorld.h"

class VoxelWorld;

class HailstormManager final
{
public:
    HailstormManager(Context & context, const Camera3D & camera, PhysicsWorld & physicsWorld, VoxelWorld & voxelWorld);

    HailstormRenderer & renderer();
    const HailstormRenderer & renderer() const;

    void addBullet(HailstormBullet bullet);

    void update(float seconds);

private:
    VoxelWorld &                    m_voxelWorld;
    Optional<HailstormRenderer>     m_renderer;
    Optional<HailstormPhysicsWorld> m_physicsWorld;
};
