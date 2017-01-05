#include "Hailstorm/HailstormManager.h"

HailstormManager::HailstormManager(Context & context, const Camera3D & camera, PhysicsWorld & physicsWorld)
{
    m_renderer.reset(context, camera);
    m_physicsWorld.reset(physicsWorld);
}

void HailstormManager::addBullet(HailstormBullet bullet, HailstormRenderer::MeshID meshID)
{
    bullet.id = m_bulletIDIncrementor;
    m_bulletIDIncrementor++;

    m_renderer->addBullet(bullet, meshID)
}

void HailstormManager::update(float seconds)
{
    m_renderer->update(seconds);
}
