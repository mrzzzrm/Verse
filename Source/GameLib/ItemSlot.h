#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include "GameLib.h"

class ItemSlot
{
public:
    ItemSlot(const glm::uvec3 & voxel, const Pose3D & pose);
    virtual ~ItemSlot() = default;

    const glm::uvec3 & voxel() const;
    const Pose3D & pose() const;

protected:
    glm::uvec3              m_voxel;
    Pose3D                  m_pose;
};