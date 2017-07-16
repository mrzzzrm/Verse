#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include "GameLib.h"
#include "R.h"
#include "VfxManager.h"
#include "VoxelDefines.h"

struct EquipmentUpdateContext;
class HailstormManager;

struct WeaponConfig
{
    float     cooldown = 0.0f;
    VfxMeshId meshID = 0;
    float     bulletSpeed = 1.0f;
    float     bulletLifetime = 1.0f;
};

class Weapon final
{
public:
    Weapon(
        const WeaponConfig & config,
        HailstormManager &   hailstormManager,
        VoxelObjectWorldUID  creatorUID);

    const WeaponConfig & config() const;

    void clearFireRequest() { m_fireRequestActive = false; }
    void setFireRequest(const glm::vec3 & direction);

    void setPose(const Pose3D & pose);

    void update(
        float                          seconds,
        const EquipmentUpdateContext & context,
        const Pose3D &                 weaponPose,
        float                          maxAngle);

private:
    WeaponConfig        m_config;
    HailstormManager &  m_hailstormManager;
    VoxelObjectWorldUID m_creatorUID;

    Pose3D m_pose;

    bool      m_fireRequestActive = false;
    glm::vec3 m_fireRequestDirection;

    float m_cooldown = 0.0f;
};