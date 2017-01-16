#include "VoxelClusterMarchingCubes.h"

#include <iostream>

#include <Deliberation/Core/ScopeProfiler.h>

VoxelClusterMarchingCubes::VoxelClusterMarchingCubes(const VoxelClusterMarchingCubesTriangulation & triangulation,
                                                     const VoxelCluster<glm::vec3> & cluster,
                                                     float scale):
        m_triangulation(triangulation),
        m_cluster(cluster),
        m_configCluster(cluster.size() + glm::uvec3(1, 1, 1)),
        m_scale(scale)
{


    auto vertexLayout = DataLayout({{"Position", Type_Vec3},
                                    {"Normal",   Type_Vec3},
                                    {"Color",    Type_Vec3}});

    m_vertices = LayoutedBlob(vertexLayout);
}

void VoxelClusterMarchingCubes::run()
{
    run(glm::uvec3(0), m_cluster.size() - glm::uvec3(1));
}

void VoxelClusterMarchingCubes::run(const glm::uvec3 & llf, const glm::uvec3 & urb)
{
    if (m_configClusterDirty) onClusterChanged(glm::uvec3(0u), m_cluster.size() - 1u);

    auto & size = m_cluster.size();
    auto config = std::bitset<8>();

    m_positions = m_vertices.field<glm::vec3>("Position").iterator();
    m_normals = m_vertices.field<glm::vec3>("Normal").iterator();
    m_colors = m_vertices.field<glm::vec3>("Color").iterator();

    auto t = urb - llf + glm::uvec3(1);
    auto numVisible = t.x * t.y * t.z;
    auto numTotal = size.x * size.y * size.z;

    std::cout << "Rendering " << (((float)numVisible * 100.0f) / (float)numTotal) << std::endl;

    {
        ScopeProfiler scopeProfiler("VoxelClusterMarchingCubes::run() - mesh generation");

        for (i32 z = llf.z; z <= urb.z + 1; z++)
        {
            for (i32 y = llf.y; y <= urb.y + 1; y++)
            {
                for (i32 x = llf.x; x <= urb.x + 1; x++)
                {
                    generateMesh(x, y, z, m_configCluster.get({x, y, z}));
                }
            }
        }
    }
}

void VoxelClusterMarchingCubes::onClusterChanged(const glm::uvec3 & llfCluster, const glm::uvec3 & urbCluster)
{
    ScopeProfiler scopeProfiler("VoxelClusterMarchingCubes::onClusterChanged()");

    auto urb = urbCluster + glm::uvec3(1);

    for (i32 z = llfCluster.z; z <= urb.z; z++)
    {
        for (i32 y = llfCluster.y; y <= urb.y; y++)
        {
            for (i32 x = llfCluster.x; x <= urb.x; x++)
            {
                /**
                 * Remove old mesh vertices from total count
                 */
                auto & currentMesh = m_triangulation.configs()[m_configCluster.get({x, y, z})];
                m_numVertices -= currentMesh.size() * 3;

                /**
                 * Update config and mesh count
                 */
                std::bitset<8> config;

                config.set(0, checkVoxel(x - 1, y - 1, z - 1));
                config.set(1, checkVoxel(x - 0, y - 1, z - 1));
                config.set(2, checkVoxel(x - 0, y - 1, z - 0));
                config.set(3, checkVoxel(x - 1, y - 1, z - 0));
                config.set(4, checkVoxel(x - 1, y - 0, z - 1));
                config.set(5, checkVoxel(x - 0, y - 0, z - 1));
                config.set(6, checkVoxel(x - 0, y - 0, z - 0));
                config.set(7, checkVoxel(x - 1, y - 0, z - 0));

                m_configCluster.set({x, y, z}, config.to_ullong());

                auto & mesh = m_triangulation.configs()[config.to_ullong()];
                m_numVertices += mesh.size() * 3;
            }
        }
    }
    m_vertices.resize(m_numVertices);

    m_configClusterDirty = false;
}

LayoutedBlob && VoxelClusterMarchingCubes::takeVertices()
{
    return std::move(m_vertices);
}

bool VoxelClusterMarchingCubes::checkVoxel(i32 x, i32 y, i32 z) const
{
    auto &size = m_cluster.size();

    if (x < 0 || y < 0 || z < 0 ||
        x >= size.x || y >= size.y || z >= size.z)
    {
        return false;
    }

    return m_cluster.test({x, y, z});
}

inline void VoxelClusterMarchingCubes::generateMesh(i32 x, i32 y, i32 z, u8 configID)
{
    auto & mesh = m_triangulation.configs()[configID];

    if (mesh.empty())
    {
        return;
    }

    for (auto & triangle : mesh)
    {
        for (uint i = 0u; i < 3u; i++)
        {
            m_positions.put((triangle.positions[i] + glm::vec3(x, y, z)) * m_scale);
            m_normals.put(triangle.normal);
            m_colors.put(getCubeColorAtCorner(x, y, z, triangle.corner));
        }
    }
}

inline glm::vec3 VoxelClusterMarchingCubes::getCubeColorAtCorner(i32 x, i32 y, i32 z, u8 corner) const
{
   // Assert(corner < 8, "Illegal corner index");

    static std::array<glm::ivec3, 8> cornerOffsets = {{
                                                              {-1, -1, -1},
                                                              {0, -1, -1},
                                                              {0, -1, 0},
                                                              {-1, -1, 0},
                                                              {-1, 0, -1},
                                                              {0, 0, -1},
                                                              {0, 0, 0},
                                                              {-1, 0, 0},
                                                      }};

    auto voxel = cornerOffsets[corner] + glm::ivec3(x, y, z);

   // Assert(voxel.x >= 0 && voxel.y >= 0 && voxel.z >= 0, "");

    return m_cluster.get(voxel);
}
