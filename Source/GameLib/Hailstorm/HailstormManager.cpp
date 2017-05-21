#include "HailstormManager.h"

#include <Deliberation/Core/ScopeProfiler.h>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/Systems/PhysicsWorldSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderSystem.h>

#include "VoxelWorld.h"
#include "ResourceManager.h"

HailstormManager::HailstormManager(World & world):
    Base(world),
    m_vfxManager(world.systemRef<RenderSystem>().renderManager(), world.systemRef<ResourceManager>()),
    m_hailstormPhysicsWorld(world.systemRef<PhysicsWorldSystem>().physicsWorld(), world.systemRef<VoxelWorld>())
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
