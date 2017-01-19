#pragma once

#include <vector>

#include "GameLib.h"
#include "VoxelShapeChunk.h"
#include "Voxel.h"

class VoxelShapeChunkTree
{
public:
    void addVoxels(const std::vector<Voxel> & voxels);
    void removeVoxels(const std::vector<glm::uvec3> & voxels);

    bool lineCast(const Ray3D & ray, glm::uvec3 & voxel) const;
};
