#include "Hailstorm/HailstormManager.h"

HailstormManager::HailstormManager(Context & context,
                                   const Camera3D & camera,
                                   PhysicsWorld & physicsWorld,
                                   VoxelWorld & voxelWorld):
    m_voxelWorld(voxelWorld)
{
    m_renderer.reset(context, camera);
    m_physicsWorld.reset(physicsWorld, m_voxelWorld);
}

HailstormRenderer & HailstormManager::renderer()
{
    return *m_renderer;
}

const HailstormRenderer & HailstormManager::renderer() const
{
    return *m_renderer;
}

void HailstormManager::addBullet(HailstormBullet bullet)
{
    m_renderer->addBullet(bullet);
    m_physicsWorld->addBullet(bullet);
}

void HailstormManager::update(float seconds)
{
    m_renderer->update(seconds);
    m_physicsWorld->update(seconds);
}
