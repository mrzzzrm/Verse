#include "VoxelObjectPrimitives.h"

VoxelObjectVoxelData BuildVoxelBlock(const VoxelWorld & voxelWorld, const glm::uvec3 & size, const glm::vec3 & color)
{
    VoxelObjectVoxelData block(voxelWorld, size);

    std::vector<Voxel> voxels;
    voxels.reserve(size.x * size.y * size.z);

    for (size_t z = 0; z < size.z; z++)
    {
        for (size_t y = 0; y < size.y; y++)
        {
            for (size_t x = 0; x < size.x; x++)
            {
                voxels.emplace_back(glm::uvec3{x, y, z}, color);
            }
        }
    }

    block.addVoxels(std::move(voxels));

    return block;
}