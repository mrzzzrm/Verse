#include "Hardpoint.h"

#include "Weapon.h"

Hardpoint::Hardpoint(const Pose3D & pose, float maxAngle):
    m_pose(pose),
    m_maxAngle(maxAngle)
{}

void Hardpoint::setFireRequest(bool active, const glm::vec3 & target)
{
    if (m_weapon) m_weapon->setFireRequest(active, target);
}

void Hardpoint::setReferencePose(const Pose3D & referencePose)
{
    m_referencePose = referencePose;
}

void Hardpoint::setWeapon(std::shared_ptr<Weapon> weapon)
{
    m_weapon = weapon;
}

void Hardpoint::update(float seconds, const EquipmentUpdateContext & context)
{
    if (!m_weapon) return;

    m_weapon->update(seconds, context, m_pose, m_maxAngle);
}