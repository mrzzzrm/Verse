#include "HailstormManager.h"

#include <Deliberation/Core/ScopeProfiler.h>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/Systems/PhysicsWorldSystem.h>
#include <Deliberation/ECS/World.h>

#include "VoxelWorld.h"
#include "ResourceManager.h"

HailstormManager::HailstormManager(
    World & world,
    const Camera3D & camera
):
    Base(world),
    m_vfxManager(world.system<ApplicationSystem>().context(), camera, world.system<ResourceManager>()),
    m_hailstormPhysicsWorld(world.system<PhysicsWorldSystem>().physicsWorld(), world.system<VoxelWorld>())
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

void HailstormManager::onUpdate(float seconds)
{
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
}

void HailstormManager::onRender()
{
    std::cout << "HailstormBegin" << std::endl;
    m_vfxManager.render();
    std::cout << "HailstormEnd" << std::endl;
}