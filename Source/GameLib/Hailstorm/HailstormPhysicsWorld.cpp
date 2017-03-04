#include "HailstormPhysicsWorld.h"

#include <Deliberation/Core/Chrono.h>
#include <Deliberation/Core/Math/Ray3D.h>

#include <Deliberation/Physics/NarrowphasePrimitiveTest.h>
#include <Deliberation/Physics/RigidBody.h>
#include <Deliberation/Physics/PhysicsWorld.h>

#include "CollisionShapeTypes.h"
#include "VoxelClusterPrimitiveTest.h"
#include "VoxelWorld.h"

HailstormPhysicsWorld::HailstormPhysicsWorld(PhysicsWorld & physicsWorld, VoxelWorld & voxelWorld):
    m_physicsWorld(physicsWorld),
    m_voxelWorld(voxelWorld)
{

}

const std::vector<HailstormBulletId> & HailstormPhysicsWorld::destroyedBullets() const
{
    return m_destroyedBullets;
}

void HailstormPhysicsWorld::addBullet(const HailstormBullet & bullet)
{
    auto index = m_bullets.insert(bullet);
    m_bullets[index].id.physicsIndex = index;
}

void HailstormPhysicsWorld::update(float seconds)
{
    m_destroyedBullets.clear();

    auto currentMillis = CurrentMillis();

    for (auto & bullet : m_bullets)
    {
        float t0 = ((float)(currentMillis - bullet.particle.birth)) / 1000.0f;
        float t1 = t0 + seconds;

        auto a = bullet.particle.origin + bullet.particle.velocity * t0;
        auto b = bullet.particle.origin + bullet.particle.velocity * t1;

        auto markedForDestruction = false;

        m_physicsWorld.lineCast(Ray3D::fromTo(a, b), [&](const RayCastIntersection &intersection) -> bool {
            auto & body = intersection.body;

            if (body->shape()->type() == (int) ::CollisionShapeType::VoxelCluster) {
                auto & voxelClusterIntersection =
                    static_cast<const RayCastVoxelClusterIntersection &>(intersection);

//                if (voxelClusterIntersection.object.lock()->id().worldUID == bullet.creator) {
//                    return true;
//                }

                auto voxelObject = voxelClusterIntersection.object.lock();
                if (voxelObject)
                {
                    m_impactSystem.process(*voxelObject, voxelClusterIntersection.voxel, 100, 2);
                }

                auto localHitPoint = glm::vec3(voxelClusterIntersection.voxel);
                auto relativeHitPoint = body->transform().pointLocalToWorld(localHitPoint) -
                    body->transform().position();

                body->applyImpulse(relativeHitPoint, bullet.particle.velocity * 0.1f);
            }

            if (!markedForDestruction) {
                m_destroyedBullets.emplace_back(bullet.id);
                markedForDestruction = true;
            }
            return false;
        });
    }

    for (const auto & bullet : m_destroyedBullets)
    {
        m_bullets.erase(bullet.physicsIndex);
    }
}