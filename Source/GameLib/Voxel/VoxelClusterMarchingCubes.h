#pragma once

#include <bitset>

#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/Optional.h>

#include <Deliberation/Scene/Vertices.h>

#include <Deliberation/Deliberation.h>

#include "Voxel/VoxelCluster.h"
#include "Voxel/VoxelClusterMarchingCubesTriangulation.h"

#include "GameLib.h"

class VoxelClusterMarchingCubes final
{
public:
    VoxelClusterMarchingCubes(const VoxelClusterMarchingCubesTriangulation & triangulation,
                              const VoxelCluster<glm::vec3> & cluster,
                              float scale = 1.0f);

    void run();

    LayoutedBlob && takeVertices();

private:
    bool checkVoxel(i32 x, i32 y, i32 z) const;
    void generateMesh(i32 x, i32 y, i32 z, u8 configID);
    glm::vec3 getCubeColorAtCorner(i32 x, i32 y, i32 z, u8 corner) const;

private:
    const VoxelClusterMarchingCubesTriangulation &  m_triangulation;
    const VoxelCluster<glm::vec3> &                 m_cluster;
    float                                           m_scale;

    LayoutedBlob                                    m_vertices;

    LayoutedBlob::Iterator<glm::vec3>               m_positions;
    LayoutedBlob::Iterator<glm::vec3>               m_normals;
    LayoutedBlob::Iterator<glm::vec3>               m_colors;
};
