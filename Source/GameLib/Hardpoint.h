#pragma once

#include <memory>

#include <Deliberation/Core/Math/Pose3D.h>

#include "GameLib.h"

class EquipmentUpdateContext;
class Weapon;

class Hardpoint final
{
public:
    Hardpoint(const Pose3D & pose, float maxAngle);

    void setFireRequest(bool active, const glm::vec3 & target);

    /**
     * Only used for warping the Hardpoint, otherwise use update()'s referencePose
     */
    void setReferencePose(const Pose3D & referencePose);

    void setWeapon(std::shared_ptr<Weapon> weapon);

    void update(float seconds, const EquipmentUpdateContext & context);

private:
    Pose3D                  m_pose;
    float                   m_maxAngle = glm::half_pi<float>();
    Pose3D                  m_referencePose;
    std::shared_ptr<Weapon> m_weapon;
};