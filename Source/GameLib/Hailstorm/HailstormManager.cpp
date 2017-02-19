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

void HailstormManager::addBullet(HailstormParticle bullet)
{
    m_renderer->addParticle(bullet);
    m_physicsWorld->addBullet(bullet);
}

void HailstormManager::addParticle(HailstormParticle bullet)
{
    m_renderer->addParticle(bullet);
}

void HailstormManager::update(float seconds)
{
    m_physicsWorld->update(seconds);

    for (auto & bulletID : m_physicsWorld->destroyedBullets())
    {
        m_renderer->removeParticle(bulletID);
    }
}

void HailstormManager::render()
{
    m_renderer->render();
}
