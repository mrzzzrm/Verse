#include "Attachment.h"

#include <Deliberation/ECS/RigidBodyComponent.h>
#include <Deliberation/ECS/Transform3DComponent.h>

Attachment::Attachment(const AttachmentDesc & desc) : m_desc(desc) {}

const glm::uvec3 & Attachment::voxel() const { return m_desc.voxel; }

const Pose3D & Attachment::localPose() const { return m_desc.pose; }

Pose3D Attachment::worldPose() const
{
    AssertM(m_entity.hasComponent<Transform3DComponent>(), "Needs Transform");

    const auto & transform = m_entity.component<Transform3DComponent>().transform();
    Pose3D       pose = m_desc.pose;
    pose.setPosition(
        pose.position() + glm::vec3(m_desc.voxel) + glm::vec3(0.5f));

    return Pose3D::fromTransformedPose(transform, pose);
}

size_t Attachment::index() const { return m_index; }

bool Attachment::enabled() const { return m_enabled; }

void Attachment::setIndex(size_t index) { m_index = index; }

void Attachment::setEnabled(bool enabled)
{
    const auto before = m_enabled;

    m_enabled = enabled;

    if (!before && enabled) onEnabled();
    if (before && !enabled) onDisabled();
}

void Attachment::setEntity(const Entity & entity)
{
    AssertM(!m_entity.isValid(), "Attachments can't switch entities") m_entity =
        entity;
}