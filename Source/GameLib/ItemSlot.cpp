#include "ItemSlot.h"

ItemSlot::ItemSlot(const glm::uvec3 & voxel, const Pose3D & pose):
    m_voxel(voxel),
    m_pose(pose)
{}

const glm::uvec3 & ItemSlot::voxel() const
{
    return m_voxel;
}

const Pose3D & ItemSlot::pose() const
{
    return m_pose;
}