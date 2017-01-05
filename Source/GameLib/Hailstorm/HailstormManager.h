#pragma once

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Physics/PhysicsWorld.h>

#include "Hailstorm/HailstormBullet.h"
#include "Hailstorm/HailstormRenderer.h"
#include "Hailstorm/HailstormPhysicsWorld.h"

class HailstormManager final
{
public:
    HailstormManager(Context & context, const Camera3D & camera, PhysicsWorld & physicsWorld);

    void addBullet(HailstormBullet bullet, HailstormRenderer::MeshID meshID);

    void update(float seconds);

private:
    Optional<HailstormRenderer>     m_renderer;
    Optional<HailstormPhysicsWorld> m_physicsWorld;

    HailstormBulletID               m_bulletIDIncrementor = 0;
};
