#include "Voxel/VoxelClusterShape.h"

AABB VoxelClusterShape::bounds(const Transform3D & transform) const
{
    return AABB();
}

glm::mat3 VoxelClusterShape::localInertia() const
{
    return glm::mat3(1.0f);
}