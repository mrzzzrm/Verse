#pragma once

#include "CollisionShapeTypes.h"

template<typename T>
VoxelClusterShape::VoxelClusterShape(const VoxelCluster<T> & cluster):
    CollisionShape((int)::CollisionShapeType::VoxelCluster)
{

}
