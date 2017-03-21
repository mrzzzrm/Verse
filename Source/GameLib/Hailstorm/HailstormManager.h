#pragma once

#include <Deliberation/ECS/System.h>

#include "GameLib.h"
#include "HailstormBullet.h"
#include "HailstormPhysicsWorld.h"
#include "VfxManager.h"

namespace deliberation
{
class PhysicsWorld;
}

class HailstormManager final:
    public System<HailstormManager>
{
public:
    HailstormManager(
        World & world,
        const Camera3D & camera);

    VfxManager & vfxManager();

    void addBullet(HailstormBullet bullet);

protected:
    void onUpdate(float seconds) override;
    void onRender() override;

private:
    VfxManager              m_vfxManager;
    HailstormPhysicsWorld   m_hailstormPhysicsWorld;
};
