#include "Attachment.h"

Attachment::Attachment(const AttachmentDesc & desc):
    m_desc(desc)
{}

const glm::uvec3 & Attachment::voxel() const
{
    return m_desc.voxel;
}

const Pose3D & Attachment::pose() const
{
    return m_desc.pose;
}

size_t Attachment::index() const
{
    return m_index;
}

bool Attachment::enabled() const
{
    return m_enabled;
}

void Attachment::setIndex(size_t index)
{
    m_index = index;
}

void Attachment::setEnabled(bool enabled)
{
    const auto before = m_enabled;

    m_enabled = enabled;

    if (!before && enabled) onEnabled();
    if (before && !enabled) onDisabled();
}

void Attachment::setEntity(const Entity & entity)
{
    Assert(!m_entity.isValid(), "Attachments can't switch entities")
    m_entity = entity;
}