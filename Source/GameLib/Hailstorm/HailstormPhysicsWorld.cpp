#include "HailstormManager.h"

#include <Deliberation/Core/Chrono.h>
#include <Deliberation/Core/Math/Ray3D.h>

#include <Deliberation/Physics/NarrowphasePrimitiveTest.h>
#include <Deliberation/Physics/RigidBody.h>

#include "CollisionShapeTypes.h"
#include "VoxelClusterPrimitiveTest.h"
#include "VoxelWorld.h"

HailstormPhysicsWorld::HailstormPhysicsWorld(PhysicsWorld & physicsWorld, VoxelWorld & voxelWorld):
    m_physicsWorld(physicsWorld),
    m_voxelWorld(voxelWorld)
{

}

const std::vector<HailstormParticleID> & HailstormPhysicsWorld::destroyedBullets() const
{
    return m_destroyedBullets;
}

void HailstormPhysicsWorld::addBullet(const HailstormParticle & bullet)
{
    auto index = m_bullets.insert(bullet);
    m_bullets[index].id.physicsWorldIndex = index;
}

void HailstormPhysicsWorld::update(float seconds)
{
    m_destroyedBullets.clear();

    auto currentMillis = CurrentMillis();

    for (auto & bullet : m_bullets)
    {
        float t0 = ((float)(currentMillis - bullet.birth)) / 1000.0f;
        float t1 = t0 + seconds;

        auto a = bullet.origin + bullet.velocity * t0;
        auto b = bullet.origin + bullet.velocity * t1;

        auto markedForDestruction = false;

        m_physicsWorld.lineCast(Ray3D::fromTo(a, b), [&](const RayCastIntersection &intersection) -> bool {
            if (intersection.body.shape()->type() == (int) ::CollisionShapeType::VoxelCluster) {
                auto & voxelClusterIntersection =
                    static_cast<const RayCastVoxelClusterIntersection &>(intersection);

                if (voxelClusterIntersection.object.lock()->id().worldUID == bullet.creator) {
                    return true;
                }

                voxelClusterIntersection.object.lock()->removeVoxels({voxelClusterIntersection.voxel});
            }

            if (!markedForDestruction) {
                m_destroyedBullets.emplace_back(bullet.id);
                markedForDestruction = true;
            }
            return false;
        });
    }

    for (auto & bulletID : m_destroyedBullets)
    {
        m_bullets.erase(bulletID.physicsWorldIndex);
    }
}