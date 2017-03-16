#include "ItemSlot.h"

ItemSlot::ItemSlot(const ItemSlotDesc & desc):
    m_desc(desc)
{}

const glm::uvec3 & ItemSlot::voxel() const
{
    return m_desc.voxel;
}

const Pose3D & ItemSlot::pose() const
{
    return m_desc.pose;
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