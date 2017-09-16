#pragma once

struct VoxelClusterSegment
{
    std::vector<glm::uvec3> voxels;
    glm::uvec3              llf =
        glm::uvec3(std::numeric_limits<glm::uvec3::value_type>::max());
    glm::uvec3 urb;
};