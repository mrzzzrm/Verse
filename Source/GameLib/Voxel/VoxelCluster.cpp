#include "VoxelCluster.h"

#include <limits>

#include "Voxel.h"

template<> const glm::vec3 VoxelCluster<glm::vec3>::EMPTY_VOXEL(255, 0, 127);
template<> const bool VoxelCluster<bool>::EMPTY_VOXEL(false);
template<> const u8 VoxelCluster<u8>::EMPTY_VOXEL(0);
template<> const u16 VoxelCluster<u16>::EMPTY_VOXEL(0);
template<> const u32 VoxelCluster<u32>::EMPTY_VOXEL(0);
template<> const Voxel VoxelCluster<Voxel>::EMPTY_VOXEL({0, 0, 0}, std::numeric_limits<u32>::max(), std::numeric_limits<float>::max());
template<> const float VoxelCluster<float>::EMPTY_VOXEL(std::numeric_limits<float>::max());