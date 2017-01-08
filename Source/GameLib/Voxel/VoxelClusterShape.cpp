#include "VoxelClusterShape.h"

#include <Deliberation/Core/Math/Morton.h>
#include <Deliberation/Core/Math/Sphere.h>
#include <Deliberation/Core/Math/PrimitiveIntersection.h>
#include <Deliberation/Core/Math/Transform3D.h>

VoxelClusterShape::VoxelClusterShape():
    CollisionShape((int)::CollisionShapeType::VoxelCluster)
{

}

AABB VoxelClusterShape::bounds(const Transform3D & transform) const
{
    return AABB();
}

glm::mat3 VoxelClusterShape::localInertia() const
{
    return glm::mat3(1.0f);
}

bool VoxelClusterShape::rayCast(const Transform3D & transform, const Ray3D & ray, glm::uvec3 & voxel) const
{
    std::vector<glm::uvec3> voxels;

    auto localOrigin = transform.pointWorldToLocal(ray.origin());
    auto localDirection = transform.directionWorldToLocal(ray.direction());

    auto normalizedRay = Ray3D(localOrigin, localDirection).normalized();

    rayCastTree(0, normalizedRay, voxels);

    if (voxels.empty())
    {
        return false;
    }

    auto minimumSquaredDistance = std::numeric_limits<float>::max();
    auto minimumDistanceVoxel = size_t(0);

    for (size_t v = 0; v < voxels.size(); v++)
    {
        auto position = glm::vec3(voxels[v]) + glm::vec3(0.5f);
        auto delta = position - ray.origin();
        auto squaredDistance = glm::dot(delta, delta);

        if (squaredDistance < minimumSquaredDistance)
        {
            minimumSquaredDistance = squaredDistance;
            minimumDistanceVoxel = v;
        }
    }

    voxel = voxels[minimumDistanceVoxel];

    return true;
}

void VoxelClusterShape::rayCastTree(u32 index, const Ray3D & ray, std::vector<glm::uvec3> & voxels) const
{
    auto & node = m_nodes[index];

    if (node.isLeaf)
    {
        if (NormalizedRay3DSphereIntersection(ray.origin(),
                                              ray.direction(),
                                              glm::vec3(node.leaf.voxel) + glm::vec3(0.5f),
                                              0.5f))
        {
            voxels.emplace_back(node.leaf.voxel);
        }
    }
    else
    {
        auto position = node.node.bounds.position();
        auto radius = node.node.bounds.radius();

        if (!NormalizedRay3DSphereIntersection(ray.origin(), ray.direction(), position, radius))
        {
            return;
        }

        auto leftIndex = index * 2 + 1;
        auto rightIndex = leftIndex + 1;

        if (m_nodeMask[leftIndex])
        {
            rayCastTree(leftIndex, ray, voxels);
        }
        if (m_nodeMask[rightIndex])
        {
            rayCastTree(rightIndex, ray, voxels);
        }
    }
}