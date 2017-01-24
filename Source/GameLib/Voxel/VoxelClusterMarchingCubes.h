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

    void onClusterChanged(const glm::uvec3 & llf, const glm::uvec3 & urb);

    void run();
    void run(const glm::uvec3 & llf, const glm::uvec3 & urb,
             const Optional<glm::vec3> & colorOverride = Optional<glm::vec3>());

    LayoutedBlob && takeVertices();

private:
    inline void generateMesh(i32 x, i32 y, i32 z, u8 configID);
    inline glm::vec3 getCubeColorAtCorner(i32 x, i32 y, i32 z, u8 corner) const;

private:
    const VoxelClusterMarchingCubesTriangulation &  m_triangulation;
    const VoxelCluster<glm::vec3> &                 m_cluster;
    VoxelCluster<u8>                                m_configCluster;
    bool                                            m_configClusterDirty = true;
    float                                           m_scale;
    size_t                                          m_maxNumVertices = 0;

    LayoutedBlob                                    m_vertices;

    LayoutedBlob::Iterator<glm::vec3>               m_positions;
    LayoutedBlob::Iterator<glm::vec3>               m_normals;
    LayoutedBlob::Iterator<glm::vec3>               m_colors;
};
