#include "HailstormManager.h"

#include <Deliberation/Core/ScopeProfiler.h>
#include <Deliberation/ECS/World.h>

HailstormManager::HailstormManager(
    World & world,
    Context & context,
    const Camera3D & camera,
    PhysicsWorld & physicsWorld,
    VoxelWorld & voxelWorld
):
    Base(world),
    m_vfxManager(context, camera, voxelWorld),
    m_hailstormPhysicsWorld(physicsWorld, voxelWorld)
{

}

DurationMicros HailstormManager::updateDuration() const
{
    return m_updateDuration;
}

VfxManager & HailstormManager::vfxManager()
{
    return m_vfxManager;
}

void HailstormManager::addBullet(HailstormBullet bullet)
{
    bullet.id.particleId = m_vfxManager.addParticle(bullet.particle);
    m_hailstormPhysicsWorld.addBullet(bullet);
}

void HailstormManager::onUpdate(float seconds)
{
    ScopeProfiler profiler;

    m_vfxManager.update(seconds);
    m_hailstormPhysicsWorld.update(seconds);

    for (const auto & modification : m_hailstormPhysicsWorld.voxelObjectModifications())
    {
        world().eventManager().emit(modification);
    }

    for (const auto & hit : m_hailstormPhysicsWorld.voxelObjectBulletHits())
    {
        world().eventManager().emit(hit);
    }

    for (auto & bullet : m_hailstormPhysicsWorld.destroyedBullets())
    {
        m_vfxManager.removeParticle(bullet.particleId);
    }

    m_updateDuration = profiler.stop();
}

void HailstormManager::onRender()
{
    m_vfxManager.render();
}