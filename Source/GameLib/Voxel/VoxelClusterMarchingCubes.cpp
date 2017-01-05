#include "VoxelClusterMarchingCubes.h"

VoxelClusterMarchingCubes::VoxelClusterMarchingCubes(const VoxelClusterMarchingCubesTriangulation & triangulation,
                                                     const VoxelCluster<glm::vec3> & cluster,
                                                     float scale):
        m_triangulation(triangulation),
        m_cluster(cluster),
        m_scale(scale)
{
}

void VoxelClusterMarchingCubes::run()
{
    auto vertexLayout = DataLayout({{"Position", Type_Vec3},
                                    {"Normal",   Type_Vec3},
                                    {"Color",    Type_Vec3}});

    m_vertices = LayoutedBlob(vertexLayout);

    m_positions.reset(m_vertices.field<glm::vec3>("Position"));
    m_normals.reset(m_vertices.field<glm::vec3>("Normal"));
    m_colors.reset(m_vertices.field<glm::vec3>("Color"));

    auto &size = m_cluster.size();
    auto config = std::bitset<8>();

    for (i32 z = 0; z <= size.z; z++)
    {
        for (i32 y = 0; y <= size.y; y++)
        {
            for (i32 x = 0; x <= size.x; x++)
            {
                config.reset();

                config.set(0, checkVoxel(x - 1, y - 1, z - 1));
                config.set(1, checkVoxel(x - 0, y - 1, z - 1));
                config.set(2, checkVoxel(x - 0, y - 1, z - 0));
                config.set(3, checkVoxel(x - 1, y - 1, z - 0));
                config.set(4, checkVoxel(x - 1, y - 0, z - 1));
                config.set(5, checkVoxel(x - 0, y - 0, z - 1));
                config.set(6, checkVoxel(x - 0, y - 0, z - 0));
                config.set(7, checkVoxel(x - 1, y - 0, z - 0));

                generateMesh(x, y, z, config.to_ullong());
            }
        }
    }
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

void VoxelClusterMarchingCubes::generateMesh(i32 x, i32 y, i32 z, u8 configID)
{
    auto & mesh = m_triangulation.configs()[configID];

    if (mesh.empty())
    {
        return;
    }

    auto vertexIndex = m_vertices.count();

    m_vertices.resize(m_vertices.count() + mesh.size() * 3);

    for (auto & triangle : mesh)
    {
        for (uint i = 0u; i < 3u; i++)
        {
            (*m_positions)[vertexIndex] = (triangle.positions[i] + glm::vec3(x, y, z)) * m_scale;
            (*m_normals)[vertexIndex] = triangle.normal;
            (*m_colors)[vertexIndex] = getCubeColorAtCorner(x, y, z, triangle.corner);
            vertexIndex++;
        }
    }
}

glm::vec3 VoxelClusterMarchingCubes::getCubeColorAtCorner(i32 x, i32 y, i32 z, u8 corner) const
{
    Assert(corner < 8, "Illegal corner index");

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

    Assert(voxel.x >= 0 && voxel.y >= 0 && voxel.z >= 0, "");

    return m_cluster.get(voxel);
}
