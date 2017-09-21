#include "AimHelper.h"

#include <Deliberation/ECS/RigidBodyComponent.h>
#include <Deliberation/ECS/Transform3DComponent.h>

#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>

#include <Deliberation/Scene/Camera3D.h>

#include "CollisionShapeTypes.h"
#include "VoxelClusterPrimitiveTest.h"

AimHelper::AimHelper(const Camera3D & camera, const PhysicsWorld & physicsWorld)
    : m_camera(camera), m_physicsWorld(physicsWorld)
{
}

AimHelperResult AimHelper::getTarget(const glm::vec2 & mouse)
{
    AimHelperResult result;

    auto mouseNearPlane = (mouse + 1.0f) / 2.0f;
    auto nearPlane = m_camera.nearPlane();
    auto mouseWorld = nearPlane.origin() +
                      mouseNearPlane.x * nearPlane.right() +
                      mouseNearPlane.y * nearPlane.up();
    auto direction = glm::normalize(mouseWorld - m_camera.position()) * 2000.0f;
    auto origin = m_camera.position();

    glm::vec3 target = origin + direction;

    result.hit = false;
    result.pointOfImpact = target;

    m_physicsWorld.lineCast(
        Ray3D(origin, direction),
        [&](const RayCastIntersection & intersection) -> bool {
            if (intersection.body->shape()->type() ==
                (int)::CollisionShapeType::VoxelCluster)
            {
                auto & voxelClusterIntersection =
                    static_cast<const RayCastVoxelClusterIntersection &>(
                        intersection);

                auto & transform = intersection.body->entity()
                                       .component<Transform3DComponent>()
                                       .transform();

                result.pointOfImpact = transform.pointLocalToWorld(
                    glm::vec3(voxelClusterIntersection.voxel));
                result.hit = true;
                result.body = intersection.body;

                return true;
            }

            return false;
        });

    return result;
}