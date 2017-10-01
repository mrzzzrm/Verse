#include "VoxelClusterMarchingCubes.h"

#include <iostream>

#include <Deliberation/Core/RandomColorGenerator.h>
#include <Deliberation/Core/StreamUtils.h>

VoxelClusterMarchingCubes::VoxelClusterMarchingCubes(
    const VoxelClusterMarchingCubesTriangulation & triangulation,
    const VoxelCluster<u32> &                      cluster,
    float                                          scale,
    const glm::vec3 &                              offset)
    : m_triangulation(triangulation)
    , m_cluster(cluster)
    , m_configCluster(cluster.size() + glm::uvec3(1, 1, 1))
    , m_scale(scale)
    , m_offset(offset)
{
    m_vertices = LayoutedBlob(vertexLayout());
}

void VoxelClusterMarchingCubes::run()
{
    run(glm::uvec3(0), m_cluster.size() - glm::uvec3(1));
}

void VoxelClusterMarchingCubes::run(
    const glm::uvec3 &          llf,
    const glm::uvec3 &          urb,
    const Optional<glm::vec3> & colorOverride)
{
    if (m_configClusterDirty)
        onClusterChanged(glm::uvec3(0u), m_cluster.size() - 1u);

    auto & size = m_cluster.size();

    m_positions = m_vertices.field<glm::vec3>("Position").iterator();
    m_normals = m_vertices.field<glm::vec3>("Normal").iterator();
    m_colorIndices = m_vertices.field<u32>("ColorIndex").iterator();

    {
        size_t actualNumVertices{0};

        auto configLineInc = m_configCluster.size().x - (urb.x - llf.x + 2);
        auto configSliceInc = (m_configCluster.size().y - (urb.y - llf.y + 2)) *
                              m_configCluster.size().x;
        auto configIndex = m_configCluster.voxelToIndex(llf);
        //        i32 clusterIndex = (i32)m_cluster.voxelToIndex(llf);
        //
        //        auto clusterLineLength = m_cluster.size().x;
        //        auto clusterSliceLength = m_cluster.size().x *
        //        m_cluster.size().y;

        for (i32 z = llf.z; z <= urb.z + 1; z++)
        {
            for (i32 y = llf.y; y <= urb.y + 1; y++)
            {
                for (i32 x = llf.x; x <= urb.x + 1; x++)
                {
                    auto   config = m_configCluster.get(configIndex);
                    auto & mesh = m_triangulation.configs()[config];
                    actualNumVertices += mesh.size() * 3;

                    for (auto & triangle : mesh)
                    {
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

                        auto cornerVoxel = cornerOffsets[triangle.corner] +
                                           glm::ivec3(x, y, z);

                        auto colorIndex = m_cluster.get(cornerVoxel);

                        for (uint i = 0u; i < 3u; i++)
                        {
                            m_positions.put(
                                (triangle.positions[i] + glm::vec3(x, y, z) +
                                 m_offset) *
                                m_scale);
                            m_normals.put(triangle.normal);
                            m_colorIndices.put(colorIndex);
                        }
                    }

                    configIndex++;
                }
                configIndex += configLineInc;
            }
            configIndex += configSliceInc;
        }

        m_vertices.resize(actualNumVertices);
    }

    //    if (colorOverride)
    //    {
    //        m_colorIndices = m_vertices.field<glm::vec3>("Color").iterator();
    //        for (size_t v = 0; v < m_vertices.count(); v++)
    //        {
    //            m_colorIndices.put(*colorOverride);
    //        }
    //    }
}

void VoxelClusterMarchingCubes::onClusterChanged(
    const glm::uvec3 & llfCluster, const glm::uvec3 & urbCluster)
{
    auto clusterWidth = urbCluster.x - llfCluster.x + 1;
    auto clusterHeight = urbCluster.y - llfCluster.y + 1;
    auto clusterDepth = urbCluster.z - llfCluster.z + 1;

    auto configWidth = clusterWidth + 1;
    auto configHeight = clusterHeight + 1;
    auto configDepth = clusterDepth + 1;

    /**
     * Remove old mesh vertices from total count
     */
    {
        auto lineInc = m_configCluster.size().x - configWidth;
        auto sliceInc = (m_configCluster.size().y - configHeight) *
                        m_configCluster.size().x;
        auto configIndex = m_configCluster.voxelToIndex(llfCluster);

        for (size_t d = 0; d < configDepth; d++)
        {
            for (size_t h = 0; h < configHeight; h++)
            {
                for (size_t w = 0; w < configWidth; w++)
                {
                    auto & currentMesh =
                        m_triangulation
                            .configs()[m_configCluster.voxels()[configIndex]];
                    m_maxNumVertices -= currentMesh.size() * 3;

                    configIndex++;
                }

                configIndex += lineInc;
            }
            configIndex += sliceInc;
        }
    }

    /**
     * Update config
     */
    {
        i32 clusterIndex = (i32)m_cluster.voxelToIndex(llfCluster);
        i32 configIndex = (i32)m_configCluster.voxelToIndex(llfCluster);

        auto clusterLineInc = m_cluster.size().x - clusterWidth;
        auto clusterSliceInc =
            (m_cluster.size().y - clusterHeight) * m_cluster.size().x;

        auto configLineInc = m_configCluster.size().x - clusterWidth;
        auto configSliceInc = (m_configCluster.size().y - clusterHeight) *
                              m_configCluster.size().x;

        auto configLineLength = m_configCluster.size().x;
        auto configSliceLength =
            m_configCluster.size().x * m_configCluster.size().y;

        auto llf = 0;
        auto lrf = 1;
        auto ulf = configLineLength;
        auto urf = configLineLength + 1;
        auto llb = llf + configSliceLength;
        auto lrb = lrf + configSliceLength;
        auto ulb = ulf + configSliceLength;
        auto urb = urf + configSliceLength;

        auto & configVoxels = m_configCluster.voxels();

        for (size_t d = 0; d < clusterDepth; d++)
        {
            for (size_t h = 0; h < clusterHeight; h++)
            {
                for (size_t w = 0; w < clusterWidth; w++)
                {
                    auto v = m_cluster.test(clusterIndex);

                    if (v)
                    {
                        configVoxels[configIndex + llf] |= 0x40;
                        configVoxels[configIndex + lrf] |= 0x80;
                        configVoxels[configIndex + ulf] |= 0x04;
                        configVoxels[configIndex + urf] |= 0x08;

                        configVoxels[configIndex + llb] |= 0x20;
                        configVoxels[configIndex + lrb] |= 0x10;
                        configVoxels[configIndex + ulb] |= 0x02;
                        configVoxels[configIndex + urb] |= 0x01;
                    }
                    else
                    {
                        configVoxels[configIndex + llf] &= ~0x40;
                        configVoxels[configIndex + lrf] &= ~0x80;
                        configVoxels[configIndex + ulf] &= ~0x04;
                        configVoxels[configIndex + urf] &= ~0x08;

                        configVoxels[configIndex + llb] &= ~0x20;
                        configVoxels[configIndex + lrb] &= ~0x10;
                        configVoxels[configIndex + ulb] &= ~0x02;
                        configVoxels[configIndex + urb] &= ~0x01;
                    }

                    clusterIndex++;
                    configIndex++;
                }
                clusterIndex += clusterLineInc;
                configIndex += configLineInc;
            }
            clusterIndex += clusterSliceInc;
            configIndex += configSliceInc;
        }
    }

    /**
     * Update vertex count
     */
    {
        auto lineInc = m_configCluster.size().x - configWidth;
        auto sliceInc = (m_configCluster.size().y - configHeight) *
                        m_configCluster.size().x;
        auto configIndex = m_configCluster.voxelToIndex(llfCluster);

        for (size_t d = 0; d < configDepth; d++)
        {
            for (size_t h = 0; h < configHeight; h++)
            {
                for (size_t w = 0; w < configWidth; w++)
                {
                    auto & mesh =
                        m_triangulation
                            .configs()[m_configCluster.get(configIndex)];
                    m_maxNumVertices += mesh.size() * 3;

                    configIndex++;
                }

                configIndex += lineInc;
            }
            configIndex += sliceInc;
        }
    }

    m_vertices.resize(m_maxNumVertices);

    m_configClusterDirty = false;
}

LayoutedBlob VoxelClusterMarchingCubes::takeVertices()
{
    auto result = std::move(m_vertices);
    m_vertices = LayoutedBlob(vertexLayout());
    return result;
}

DataLayout VoxelClusterMarchingCubes::vertexLayout()
{
    return DataLayout({{"Position", Type_Vec3},
                       {"Normal", Type_Vec3},
                       {"ColorIndex", Type_U32}});
}

inline void
VoxelClusterMarchingCubes::generateMesh(i32 x, i32 y, i32 z, u8 configID)
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
            m_positions.put(
                (triangle.positions[i] + glm::vec3(x, y, z)) * m_scale);
            m_normals.put(triangle.normal);
            m_colorIndices.put(
                getCubeColorIndexAtCorner(x, y, z, triangle.corner));
        }
    }
}

inline u32 VoxelClusterMarchingCubes::getCubeColorIndexAtCorner(
    i32 x, i32 y, i32 z, u8 corner) const
{
    // AssertM(corner < 8, "Illegal corner index");

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

    // Assert(voxel.x >= 0 && voxel.y >= 0 && voxel.z >= 0);

    return m_cluster.get(voxel);
}
