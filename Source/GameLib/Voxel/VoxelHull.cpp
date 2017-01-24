#include "VoxelHull.h"

#include <sstream>

namespace
{
constexpr u8 VOXEL_IS_SET_BIT = 0x80;
}

VoxelHull::VoxelHull(const glm::uvec3 & size):
    m_cluster(size + glm::uvec3(2))
{}

const std::vector<glm::uvec3> & VoxelHull::newHullVoxels() const
{
    return m_newHullVoxels;
}

const std::vector<glm::uvec3> & VoxelHull::newObscuredVoxels() const
{
    return m_newObscuredVoxels;
}

void VoxelHull::addVoxels(const std::vector<Voxel> & voxels)
{
    m_newHullVoxels.clear();
    m_newObscuredVoxels.clear();
    m_voxelCache.clear();

    for (auto & voxel : voxels)
    {
        i32 index = m_cluster.voxelToIndex(voxel.cell + glm::uvec3(1));

        m_cluster.set(index, m_cluster.get(index) | VOXEL_IS_SET_BIT);

        incVoxel(index + 1);
        incVoxel(index - 1);
        incVoxel(index + m_cluster.lineLength());
        incVoxel(index - m_cluster.lineLength());
        incVoxel(index + m_cluster.sliceLength());
        incVoxel(index - m_cluster.sliceLength());
    }

    for (auto & voxel : voxels)
    {
        i32 index = m_cluster.voxelToIndex(voxel.cell);
        if ((m_cluster.get(index) & ~VOXEL_IS_SET_BIT) < 6) m_newHullVoxels.emplace_back(voxel.cell);
    }
}

void VoxelHull::removeVoxels(const std::vector<glm::uvec3> & voxels)
{
    m_newHullVoxels.clear();
    m_newObscuredVoxels.clear();
    m_voxelCache.clear();

    for (auto & voxel : voxels)
    {
        i32 index = m_cluster.voxelToIndex(voxel + glm::uvec3(1));

        m_cluster.set(index, m_cluster.get(index) & ~VOXEL_IS_SET_BIT);

        decVoxel(index + 1);
        decVoxel(index - 1);
        decVoxel(index + m_cluster.lineLength());
        decVoxel(index - m_cluster.lineLength());
        decVoxel(index + m_cluster.sliceLength());
        decVoxel(index - m_cluster.sliceLength());
    }

    size_t w = 0;
    for (size_t v = 0; v < m_newHullVoxels.size(); v++)
    {
         auto & voxel = m_newHullVoxels[v];

        if (m_cluster.get(voxel + glm::uvec3(1)) & VOXEL_IS_SET_BIT)
        {
            m_newHullVoxels[w] = voxel;
            w++;
        }
    }
    m_newHullVoxels.resize(w);
}

bool VoxelHull::isHullVoxel(const glm::uvec3 & voxel)
{
    return (m_cluster.get(voxel + glm::uvec3(1)) & ~VOXEL_IS_SET_BIT) < 6;
}

std::string VoxelHull::toString() const
{
    std::stringstream stream;

    for (size_t z = 0; z < m_cluster.size().z; z++)
    {
        for (size_t y = 0; y < m_cluster.size().y; y++)
        {
            for (size_t x = 0; x < m_cluster.size().x; x++)
            {
                stream << "[" << (m_cluster.get({x, y, z}) & 0x7F) << "" << ((m_cluster.get({x, y, z}) & 0x80)?"-":"X")<<"]";
            }
            stream << std::endl;
        }

        stream << std::endl;
    }

    return stream.str();
}

void VoxelHull::incVoxel(i32 index)
{
    if (!m_cluster.contains(index)) return;

    m_cluster.get(index)++;

    if ((m_cluster.get(index) & 0x7F) == 6)
    {
        m_newObscuredVoxels.emplace_back(m_cluster.indexToVoxel(index) - glm::uvec3(1));
    }

}

void VoxelHull::decVoxel(i32 index)
{
    if (!m_cluster.contains(index)) return;

    m_cluster.get(index)--;

    if ((m_cluster.get(index) & 0x7F) == 5)
    {
        m_newHullVoxels.emplace_back(m_cluster.indexToVoxel(index) - glm::uvec3(1));
    }
}