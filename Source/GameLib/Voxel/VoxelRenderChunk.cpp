#include "VoxelRenderChunk.h"

#include <bitset>

#include <glm/gtx/extented_min_max.hpp>

#include <Deliberation/Core/Assert.h>

#include "VoxelClusterMarchingCubes.h"
#include "VoxelWorld.h"

VoxelRenderChunk::VoxelRenderChunk(const glm::uvec3 & size):
    m_cluster(size),
    m_configCluster(size - glm::uvec3(1))
{

}

void VoxelRenderChunk::addVoxel(const Voxel & voxel)
{
    Assert(!m_cluster.test(voxel.cell), "Voxel already added");

    m_cluster.set(voxel.cell, voxel.color);

    m_llfDirty = glm::min(m_llfDirty, voxel.cell);
    m_urbDirty = glm::max(m_urbDirty, voxel.cell);

    m_drawDirty = true;
    m_voxelCount++;
}

void VoxelRenderChunk::removeVoxel(const glm::uvec3 & voxel)
{
    Assert(m_cluster.test(voxel), "No voxel to remove");

    m_cluster.set(voxel, decltype(m_cluster)::EMPTY_VOXEL);

    m_llfDirty = glm::min(m_llfDirty, voxel);
    m_urbDirty = glm::max(m_urbDirty, voxel);

    m_drawDirty = true;
    m_voxelCount--;
}

std::shared_ptr<VoxelRenderChunk> VoxelRenderChunk::clone()
{

}

void VoxelRenderChunk::schedule(const VoxelWorld & voxelWorld, const Pose3D & pose)
{
    if (m_voxelCount == 0) return;

    if (m_drawDirty)
    {
        auto llfConfig = glm::max(glm::uvec3(0), m_llfDirty - 1);
        auto urbConfig = glm::min(m_configCluster.size() - 1, m_urbDirty);

        auto checkVoxel = [] (i32 x, i32 y, i32 z) 
        {
            auto &size = m_cluster.size();

            if (x < 0 || y < 0 || z < 0 ||
                x >= size.x || y >= size.y || z >= size.z)
            {
                return false;
            }

            return m_cluster.test({x, y, z});
        };
        
        for (i32 z = llfConfig.z; z <= urbConfig.z; z++)
        {
            for (i32 y = llfConfig.y; y <= urbConfig.y; y++)
            {
                for (i32 x = llfConfig.x; x <= urbConfig.x; x++)
                {
                    auto config = std::bitset<8>();

                    config.set(0, checkVoxel(x - 0, y - 0, z - 0));
                    config.set(1, checkVoxel(x + 1, y - 0, z - 0));
                    config.set(2, checkVoxel(x + 1, y - 0, z + 1));
                    config.set(3, checkVoxel(x - 0, y - 0, z + 1));
                    config.set(4, checkVoxel(x - 0, y + 1, z - 0));
                    config.set(5, checkVoxel(x + 1, y + 1, z - 0));
                    config.set(6, checkVoxel(x + 1, y + 1, z + 1));
                    config.set(7, checkVoxel(x - 0, y + 1, z + 1));

                    m_configCluster.set({x, y, z}, config.to_ullong());
                }
            }
        }

        auto marchingCubes = VoxelClusterMarchingCubes(voxelWorld.marchingCubesTriangulation(), m_cluster,
                                                       m_configCluster, 1.0f);
        marchingCubes.run();

        m_drawDirty = false;
    }

    m_transformUniform.set(pose.matrix());
    m_draw.schedule();
}
