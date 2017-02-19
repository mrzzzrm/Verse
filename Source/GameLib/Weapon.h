#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include "GameLib.h"
#include "HailstormManager.h"
#include "VoxelDefines.h"

struct EquipmentUpdateContext;

struct WeaponConfig
{
    float             cooldown = 0.0f;
    HailstormMeshID   meshID = 0;
};

class Weapon final
{
public:
    Weapon(const WeaponConfig & config,
           HailstormManager & hailstormManager,
           VoxelObjectWorldUID creatorUID);

    void setFireRequest(bool active,
                        const glm::vec3 & target);

    void setPose(const Pose3D & pose);

    void update(float seconds, const EquipmentUpdateContext & context, const Pose3D & weaponPose, float maxAngle);

private:
    WeaponConfig        m_config;
    HailstormManager &  m_hailstormManager;
    VoxelObjectWorldUID m_creatorUID;

    Pose3D              m_pose;

    bool                m_fireRequestActive = false;
    glm::vec3           m_fireRequestTarget;

    float               m_cooldown = 0.0f;
};