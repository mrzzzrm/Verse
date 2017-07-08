#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Pose3D.h>
#include <Deliberation/ECS/Entity.h>

#include "GameLib.h"

class Equipment;

struct AttachmentDesc
{
    glm::uvec3  voxel;
    Pose3D      pose;
};

class Attachment
{
public:
    Attachment(const AttachmentDesc & desc);
    virtual ~Attachment() = default;

    const glm::uvec3 & voxel() const;
    const Pose3D & localPose() const;
    Pose3D worldPose() const;
    size_t index() const;
    bool enabled() const;
    Entity entity() const { return m_entity; }

    void setIndex(size_t index);
    void setEnabled(bool enabled);
    void setEntity(const Entity & entity);

    virtual void onUpdate(float seconds) {}

protected:
    virtual void onEnabled() {}
    virtual void onDisabled() {}

protected:
    AttachmentDesc  m_desc;
    Entity          m_entity;
    size_t          m_index = 0;
    bool            m_enabled = false;
};