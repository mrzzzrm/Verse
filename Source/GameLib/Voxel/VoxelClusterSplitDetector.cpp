#include "VoxelClusterSplitDetector.h"

#include "VoxelObjectVoxelData.h"

#define VERBOSE 1

VoxelClusterSplitDetector::VoxelClusterSplitDetector(const glm::uvec3 & size):
    m_cluster(size)
{
}

const std::vector<VoxelClusterSplit> & VoxelClusterSplitDetector::splits() const
{
    return m_splits;
}

size_t VoxelClusterSplitDetector::splitContainingCrucialVoxel() const
{
    return m_splitContainingCrucialVoxel;
}

void VoxelClusterSplitDetector::setCrucialVoxel(const boost::optional<glm::uvec3> & crucialVoxel)
{
    m_crucialVoxel = crucialVoxel;
}

void VoxelClusterSplitDetector::addVoxels(const std::vector<Voxel> & voxels)
{
    for (const auto & voxel : voxels)
    {
        m_cluster.set(voxel.cell, m_currentFloodBeginId);
    }

    m_numVoxels += voxels.size();
}

void VoxelClusterSplitDetector::removeVoxels(const std::vector<glm::uvec3> & voxels)
{
    for (const auto & voxel : voxels)
    {
        m_cluster.set(voxel, VoxelCluster<u32>::EMPTY_VOXEL);
    }

    Assert(m_numVoxels >= voxels.size(), "");
    m_numVoxels -= voxels.size();
}

void VoxelClusterSplitDetector::performSplitDetection()
{
#if VERBOSE
    std::cout << "VoxelClusterSplitDetector(" << this << ")::performSplitDetection()" << std::endl;

#endif

    m_splits.clear();
    m_currentFloodBeginId = m_currentFloodId + 1;
    m_currentFloodId = m_currentFloodBeginId;

    for (u32 z = 0; z < m_cluster.size().z; z++)
    {
        for (u32 y = 0; y < m_cluster.size().y; y++)
        {
            for (u32 x = 0; x < m_cluster.size().x; x++)
            {
                const auto value = m_cluster.get({x, y, z});

                if (value == VoxelCluster<u32>::EMPTY_VOXEL) continue;

                if (value < m_currentFloodBeginId)
                {
                   // std::cout << " Flooding " << x << " " << y << " " << z << ". With: " << m_currentFloodId << std::endl;
                    flood({x, y, z});
                }
                else
                {
                  //  std::cout << " Not flooding " << x << " " << y << " " << z << ". " << value << " vs " <<  m_currentFloodBeginId << std::endl;
                }

            }
        }
    }

    if (m_crucialVoxel)
    {
        if (m_cluster.test(*m_crucialVoxel))
        {
            m_splitContainingCrucialVoxel = m_cluster.get(*m_crucialVoxel) - m_currentFloodBeginId;
        }
    }

    Assert(!m_splits.empty() || m_numVoxels == 0, "");
    Assert(m_currentFloodId - m_currentFloodBeginId == m_splits.size(), "");
}

void VoxelClusterSplitDetector::flood(const glm::uvec3 & startVoxel)
{
    visit(startVoxel);

    const auto currentSplitIndex = m_currentFloodId - m_currentFloodBeginId;

    if (currentSplitIndex >= m_splits.size()) m_splits.emplace_back();

    while (!m_floodStack.empty())
    {
        const auto voxel = m_floodStack.top();
        m_floodStack.pop();

        m_cluster.set(voxel, m_currentFloodId);

        auto & split = m_splits[currentSplitIndex];
        split.voxels.emplace_back(voxel);
        split.llf = glm::min(split.llf, voxel);
        split.urb = glm::max(split.urb, voxel);

        i32 x = (i32)voxel.x;
        i32 y = (i32)voxel.y;
        i32 z = (i32)voxel.z;

        visit({x + 1, y, z});
        visit({x - 1, y, z});
        visit({x, y + 1, z});
        visit({x, y - 1, z});
        visit({x, y, z + 1});
        visit({x, y, z - 1});
//        visit({x + 1, y + 1, z});
//        visit({x + 1, y - 1, z});
//        visit({x - 1, y + 1, z});
//        visit({x - 1, y - 1, z});
//        visit({x + 1, y, z + 1});
//        visit({x + 1, y, z - 1});
//        visit({x - 1, y, z + 1});
//        visit({x - 1, y, z - 1});
//        visit({x, y + 1, z + 1});
//        visit({x, y + 1, z - 1});
//        visit({x, y - 1, z + 1});
//        visit({x, y - 1, z - 1});
    }

    m_currentFloodId++;
    std::cout << "  Incremented flood id: " << m_currentFloodId << std::endl;
}

void VoxelClusterSplitDetector::visit(const glm::ivec3 & voxel)
{
    if (voxel.x < 0 || voxel.x >= m_cluster.size().x ||
        voxel.y < 0 || voxel.y >= m_cluster.size().y ||
        voxel.z < 0 || voxel.z >= m_cluster.size().z)
    {
        return;
    }

    const auto value = m_cluster.get(voxel);

    if (value == VoxelCluster<u32>::EMPTY_VOXEL) return;

    if (value < m_currentFloodBeginId)
    {
        m_cluster.set(voxel, m_currentFloodId);
        m_floodStack.push(glm::uvec3(voxel));
    }
}