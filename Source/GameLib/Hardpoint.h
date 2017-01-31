#pragma once

#include <memory>

#include <Deliberation/Core/Math/Pose3D.h>

#include "GameLib.h"

class Weapon;

class Hardpoint final
{
public:
    Hardpoint(const Pose3D & pose);

    void setFireRequest(bool active, const glm::vec3 & target);

    /**
     * Only used for warping the Hardpoint, otherwise use update()'s referencePose
     */
    void setReferencePose(const Pose3D & referencePose);

    void setWeapon(std::shared_ptr<Weapon> weapon);

    void update(float seconds, const Pose3D & referencePose);

private:
    Pose3D                  m_pose;
    Pose3D                  m_referencePose;
    std::shared_ptr<Weapon> m_weapon;
};