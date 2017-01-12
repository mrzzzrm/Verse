#include "VoxelClusterShape.h"

#include <functional>
#include <iostream>

#include <Deliberation/Core/Math/PrimitiveIntersection.h>
#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/StreamUtils.h>

VoxelClusterShape::VoxelClusterShape():
    CollisionShape((int)::CollisionShapeType::VoxelCluster)
{

}

AABB VoxelClusterShape::bounds(const Transform3D & transform) const
{
    const auto halfSize = glm::vec3(m_size) / 2.0f;
    const auto center = transform.pointLocalToWorld(halfSize);
    const auto radius = glm::length(halfSize);
    return AABB(center - glm::vec3(radius), center + glm::vec3(radius));
}

glm::mat3 VoxelClusterShape::localInertia() const
{
    return glm::mat3(1.0f);
}

std::string VoxelClusterShape::toString() const
{
    std::stringstream stream;

    std::function<void(u32,u32)> nodeToString = [&] (u32 index, u32 depth)
    {
        for (u32 d = 0; d < depth; d++)
        {
            stream << "  ";
        }

        if (m_nodes[index].isLeaf)
        {
            stream << "Voxel@" << m_nodes[index].leaf.voxel;
            stream << std::endl;
        }
        else
        {
            stream << index << " " << m_nodes[index].node.bounds;
            stream << std::endl;
            if (m_nodeMask[index * 2 + 1])
            {
                nodeToString(index * 2 + 1, depth + 1);
            }
            if (m_nodeMask[index * 2 + 2])
            {
                nodeToString(index * 2 + 2, depth + 1);
            }
        }
    };

    stream << "Num Nodes: " << m_nodes.size() << std::endl;

   // nodeToString(0, 0);

    return stream.str();
}

bool VoxelClusterShape::lineCast(const Transform3D & transform, const Ray3D & ray, glm::uvec3 & voxel) const
{
    std::vector<glm::uvec3> voxels;

    auto localOrigin = transform.pointWorldToLocal(ray.origin());
    auto localDirection = transform.directionWorldToLocal(ray.direction());

    lineCastTree(0, Ray3D(localOrigin, localDirection), voxels);

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

void VoxelClusterShape::lineCastTree(u32 index, const Ray3D & ray, std::vector<glm::uvec3> & voxels) const
{
    auto & node = m_nodes[index];

    if (node.isLeaf)
    {
        if (LineSphereIntersection(ray.origin(),
                                   ray.direction(),
                                    glm::vec3(node.leaf.voxel) + glm::vec3(0.5f),
                                    0.8f))
        {
            voxels.emplace_back(node.leaf.voxel);
        }
    }
    else
    {
        auto position = node.node.bounds.position();
        auto radius = node.node.bounds.radius();

        if (!LineSphereIntersection(ray.origin(), ray.direction(), position, radius))
        {
            return;
        }

        auto leftIndex = index * 2 + 1;
        auto rightIndex = leftIndex + 1;

        if (m_nodeMask[leftIndex])
        {
            lineCastTree(leftIndex, ray, voxels);
        }
        if (m_nodeMask[rightIndex])
        {
            lineCastTree(rightIndex, ray, voxels);
        }
    }
}