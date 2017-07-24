#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include "GameLib.h"
#include "R.h"
#include "VfxManager.h"
#include "VoxelDefines.h"
#include "WeaponPrototype.h"

struct EquipmentUpdateContext;
class HailstormManager;

class Weapon final
{
public:
    Weapon(
        const std::shared_ptr<const WeaponPrototype> & prototype,
        HailstormManager &   hailstormManager);

    const std::shared_ptr<const WeaponPrototype> & prototype() const { return m_prototype; }

    void clearFireRequest() { m_fireRequestActive = false; }
    void setFireRequest(const glm::vec3 & direction);

    void setPose(const Pose3D & pose);

    void update(
        float                          seconds,
        const EquipmentUpdateContext & context,
        const Pose3D &                 weaponPose,
        float                          maxAngle);

private:
    std::shared_ptr<const WeaponPrototype>    m_prototype;
    HailstormManager &                        m_hailstormManager;
    Pose3D                                    m_pose;

    bool      m_fireRequestActive = false;
    glm::vec3 m_fireRequestDirection;

    float m_cooldown = 0.0f;
};