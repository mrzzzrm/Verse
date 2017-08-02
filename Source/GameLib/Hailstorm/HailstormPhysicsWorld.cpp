#include "HailstormPhysicsWorld.h"

#include <Deliberation/Core/Chrono.h>
#include <Deliberation/Core/Math/Ray3D.h>

#include <Deliberation/Physics/NarrowphasePrimitiveTest.h>
#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>

#include "CollisionShapeTypes.h"
#include "VoxelClusterPrimitiveTest.h"
#include "VoxelWorld.h"

HailstormPhysicsWorld::HailstormPhysicsWorld(
    PhysicsWorld & physicsWorld, VoxelWorld & voxelWorld)
    : m_physicsWorld(physicsWorld), m_voxelWorld(voxelWorld)
{
}

const std::vector<HailstormBulletId> &
HailstormPhysicsWorld::destroyedBullets() const
{
    return m_destroyedBullets;
}

const std::vector<VoxelObjectBulletHit> &
HailstormPhysicsWorld::voxelObjectBulletHits() const
{
    return m_voxelObjectBulletHits;
}

void HailstormPhysicsWorld::addBullet(const HailstormBullet & bullet)
{
    auto index = m_bullets.insert(bullet);
    m_bullets[index].id.physicsIndex = index;
}

void HailstormPhysicsWorld::update(float seconds)
{
    m_destroyedBullets.clear();
    m_expiredBullets.clear();
    m_voxelObjectBulletHits.clear();

    auto currentMillis = CurrentMillis();

    for (auto & bullet : m_bullets)
    {
        float t0 = ((float)(currentMillis - bullet.particle.birth)) / 1000.0f;
        float t1 = t0 + seconds;

        auto a = bullet.particle.origin + bullet.particle.velocity * t0;
        auto b = bullet.particle.origin + bullet.particle.velocity * t1;

        auto markedForDestruction = false;

        if (currentMillis > bullet.particle.birth + bullet.particle.lifetime)
        {
            m_expiredBullets.emplace_back(bullet.id);
            continue;
        }

        m_physicsWorld.lineCast(
            Ray3D::fromTo(a, b),
            [&](const RayCastIntersection & intersection) -> bool {
                auto & body = intersection.body;

                if (body->shape()->type() ==
                    (int)::CollisionShapeType::VoxelCluster)
                {
                    auto & voxelClusterIntersection =
                        static_cast<const RayCastVoxelClusterIntersection &>(
                            intersection);

                    auto & entity = body->entity();
                    if (entity.id() == bullet.creator &&
                        bullet.creator != ECS_INVALID_ENTITY_ID)
                    {
                        return true;
                    }

                    auto & voxelObject = entity.component<VoxelObject>();
                    voxelObject.processImpact(
                        voxelClusterIntersection.voxel, 100, 2);

                    m_voxelObjectBulletHits.emplace_back(
                        entity, voxelClusterIntersection.voxel);

                    auto localHitPoint =
                        glm::vec3(voxelClusterIntersection.voxel);
                    auto relativeHitPoint =
                        body->transform().pointLocalToWorld(localHitPoint) -
                        body->transform().position();

                    body->applyImpulse(
                        relativeHitPoint, bullet.particle.velocity * 0.1f);
                }

                if (!markedForDestruction)
                {
                    m_destroyedBullets.emplace_back(bullet.id);
                    markedForDestruction = true;
                }
                return false;
            });
    }

    /**
     * Remove expired and destroyed bullets
     */
    for (const auto & bullet : m_destroyedBullets)
    {
        m_bullets.erase(bullet.physicsIndex);
    }
    for (const auto & bullet : m_expiredBullets)
    {
        m_bullets.erase(bullet.physicsIndex);
    }
}