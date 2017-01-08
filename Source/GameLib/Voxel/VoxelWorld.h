#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "VoxelObject.h"

class VoxelObject;
class VoxelObjectID;

class VoxelWorld final
{
public:
    VoxelWorld();

    VoxelObjectID addVoxelObject(std::unique_ptr<VoxelObject> voxelObject);

    void removeVoxel(const VoxelObjectID & voxelObjectID, const glm::uvec3 & voxel);
};