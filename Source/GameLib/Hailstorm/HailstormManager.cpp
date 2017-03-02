#include "HailstormManager.h"

HailstormManager::HailstormManager(
    Context & context,
    const Camera3D & camera,
    PhysicsWorld & physicsWorld,
    VoxelWorld & voxelWorld
):
    m_vfxManager(context, camera, voxelWorld),
    m_hailstormPhysicsWorld(physicsWorld, voxelWorld)
{

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

void HailstormManager::update(float seconds)
{
    m_vfxManager.update(seconds);
    m_hailstormPhysicsWorld.update(seconds);

    for (auto & bullet : m_hailstormPhysicsWorld.destroyedBullets())
    {
        m_vfxManager.removeParticle(bullet.particleId);
    }
}

void HailstormManager::render()
{
    m_vfxManager.render();
}