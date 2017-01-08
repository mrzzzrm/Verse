#include "VoxelClusterPrimitiveTest.h"

#include "VoxelClusterShape.h"
#include "VoxelRigidBodyPayload.h"

RayCastVoxelClusterIntersection::RayCastVoxelClusterIntersection(const RigidBody & body):
    RayCastIntersection(body)
{

}

std::unique_ptr<RayCastIntersection> VoxelClusterPrimitiveTest::rayCast(const Ray3D & ray,
                                                                        const RigidBody & body) const
{
    auto shape = body.shape();

    auto voxelClusterShape = std::dynamic_pointer_cast<VoxelClusterShape>(shape);

    glm::uvec3 voxel;
    if (voxelClusterShape->rayCast(body.transform(), ray, voxel))
    {
        auto & payload = body.payload();
        auto & voxelPayload = dynamic_cast<VoxelRigidBodyPayload&>(*payload);

        auto intersection = std::make_unique<RayCastVoxelClusterIntersection>(body);
        intersection->voxel = voxel;
        intersection->voxelObjectID = voxelPayload.voxelObjectID;

        return std::unique_ptr<RayCastIntersection>(std::move(intersection));
    }

    return nullptr;
}