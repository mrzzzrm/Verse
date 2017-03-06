#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include "GameLib.h"

class Equipment;

class ItemSlot
{
public:
    ItemSlot(const glm::uvec3 & voxel, const Pose3D & pose);
    virtual ~ItemSlot() = default;

    const glm::uvec3 & voxel() const;
    const Pose3D & pose() const;
    size_t index() const;
    bool enabled() const;

    void setIndex(size_t index);
    void setEnabled(bool enabled);

protected:
    virtual void onEnabled();
    virtual void onDisabled();

protected:
    glm::uvec3  m_voxel;
    Pose3D      m_pose;
    size_t      m_index = 0;
    bool        m_enabled = true;
};