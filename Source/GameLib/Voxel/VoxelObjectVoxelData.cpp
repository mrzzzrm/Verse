//#include "VoxelObjectVoxelData.h"
//
//#include <glm/gtx/extented_min_max.hpp>
//
//VoxelObjectVoxelData::VoxelObjectVoxelData(const glm::uvec3 & size):
//    m_size(size),
//    m_visibilityCluster(size)
//{
//
//}
//
//void VoxelObjectVoxelData::addVoxels(std::vector<Voxel> voxels)
//{
//    if (voxels.empty()) return;
//
//    /**
//     * Set visibility to NotSet
//     */
//    glm::ivec3 llf(voxels[0]);
//    glm::ivec3 urb(voxels[0]);
//    for (auto & voxel : voxels)
//    {
//        llf = glm::min(llf, voxel.cell);
//        urb = glm::max(urb, voxel.cell);
//
//        m_visibilityCluster.set(voxel.cell, VoxelVisibilityState::NotSet);
//    }
//
//    /**
//     * Update visibility of all possibly affected voxels
//     */
//    llf = glm::max(llf - glm::ivec3(1), {0});
//    urb = glm::min(urb + glm::ivec3(1), m_size - glm::ivec3(1));
//    for (i32 z = llf.z; z <= urb.z; z++)
//    {
//        for (i32 y = llf.y; y <= urb.y; y++)
//        {
//            for (i32 x = llf.x; x <= urb.x; x++)
//            {
//                if (m_visibilityCluster.get({x, y, z}) == VoxelVisibilityState::NoVoxel) continue;
//
//                m_visibilityCluster.set({x, y, z}, VoxelVisibilityState::Hidden);
//
//                for (i32 rz = -1; rz <= 1; rz++)
//                {
//                    for (i32 ry = -1; ry <= 1; ry++)
//                    {
//                        for (i32 rx = -1; rx <= 1; rx++)
//                        {
//                            if (rx == 0 && ry == 0 && rz == 0) continue;
//
//                            glm::ivec3 neighbour(x + rx, y + ry, z + rz);
//
//                            if (neighbour.x < 0 || neighbour.x >= m_size.x) continue;
//                            if (neighbour.y < 0 || neighbour.y >= m_size.y) continue;
//                            if (neighbour.z < 0 || neighbour.z >= m_size.z) continue;
//
//                            if (m_visibilityCluster.get(neighbour) != VoxelVisibilityState::NoVoxel)
//                            {
//                                m_visibilityCluster.set({x, y, z}, VoxelVisibilityState::Visible);
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//    /**
//     * Update visibility of voxels
//     */
//    for (auto & voxel : voxels)
//    {
//        voxel.visible = m_visibilityCluster.get({x, y, z}) == VoxelVisibilityState::Visible;
//    }
//
//    /**
//     * Update chunks
//     */
//    m_renderTree.addVoxels(voxels);
//    m_shapeTree.addVoxels(voxels);
//}
//
//void VoxelObjectVoxelData::removeVoxels(const std::vector<glm::uvec3> & voxels)
//{
//
//}
//
//const glm::uvec3 & VoxelObjectVoxelData::size() const
//{
//
//}
