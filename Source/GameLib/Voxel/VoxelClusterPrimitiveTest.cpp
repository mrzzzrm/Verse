#include "VoxelClusterPrimitiveTest.h"

#include "VoxelRigidBodyPayload.h"

RayCastVoxelClusterIntersection::RayCastVoxelClusterIntersection(const RigidBody & body):
    RayCastIntersection(body)
{

}

std::unique_ptr<RayCastIntersection> VoxelClusterPrimitiveTest::lineTest(const Ray3D & ray,
                                                                         const RigidBody & body) const
{
    auto shape = body.shape();

    auto voxelClusterShape = std::dynamic_pointer_cast<VoxelShapeTree>(shape);

    glm::uvec3 voxel;
    if (voxelClusterShape->lineCast(body.transform(), ray, voxel))
    {
        auto & payload = body.payload();
        auto & voxelPayload = dynamic_cast<VoxelRigidBodyPayload&>(*payload);

        auto intersection = std::make_unique<RayCastVoxelClusterIntersection>(body);
        intersection->voxel = voxel;
        intersection->object = voxelPayload.object;

        return std::unique_ptr<RayCastIntersection>(std::move(intersection));
    }

    return nullptr;
}