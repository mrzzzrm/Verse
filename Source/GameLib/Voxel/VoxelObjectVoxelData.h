#pragma once

#include <glm/glm.hpp>

#include "GameLib.h"
#include "Voxel.h"
#include "VoxelHull.h"
#include "VoxelRenderChunkTree.h"
#include "VoxelShapeTree.h"

class VoxelWorld;

class VoxelObjectVoxelData final
{
public:
    VoxelObjectVoxelData(const VoxelWorld & voxelWorld, const glm::uvec3 & size);

    const VoxelWorld & voxelWorld() const;
    const glm::uvec3 & size() const;
    const VoxelRenderChunkTree & renderTree() const;
    const VoxelShapeTree & shapeTree() const;
    const VoxelHull & hull() const;

    void addVoxels(std::vector<Voxel> voxels);
    void removeVoxels(const std::vector<glm::uvec3> & voxels);

private:
    const VoxelWorld &    m_voxelWorld;
    glm::uvec3            m_size;
    VoxelRenderChunkTree  m_renderTree;
    VoxelShapeTree        m_shapeTree;
    VoxelHull             m_hull;
};