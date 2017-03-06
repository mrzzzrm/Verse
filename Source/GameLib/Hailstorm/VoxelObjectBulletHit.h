#pragma once

#include <memory>

#include "GameLib.h"

class VoxelObject;

struct VoxelObjectBulletHit
{
    VoxelObjectBulletHit(std::shared_ptr<VoxelObject> object, const glm::uvec3 & voxel):
        object(object), voxel(voxel)
    {}

    std::shared_ptr<VoxelObject>    object;
    glm::uvec3                      voxel;
};