#include "VoxelCluster.h"

template<> const glm::vec3 VoxelCluster<glm::vec3>::EMPTY_VOXEL(255, 0, 127);
template<> const bool VoxelCluster<bool>::EMPTY_VOXEL(false);
template<> const u8 VoxelCluster<u8>::EMPTY_VOXEL(0);
