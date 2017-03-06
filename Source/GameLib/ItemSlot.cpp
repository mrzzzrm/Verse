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

size_t ItemSlot::index() const
{
    return m_index;
}

bool ItemSlot::enabled() const
{
    return m_enabled;
}

void ItemSlot::setIndex(size_t index)
{
    m_index = index;
}

void ItemSlot::setEnabled(bool enabled)
{
    const auto before = m_enabled;

    m_enabled = enabled;

    if (!before && enabled) onEnabled();
    if (before && !enabled) onDisabled();
}

void ItemSlot::onEnabled()
{

}

void ItemSlot::onDisabled()
{

}