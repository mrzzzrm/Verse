#include "Hardpoint.h"

#include <Deliberation/Core/StreamUtils.h>

#include "Equipment.h"
#include "Weapon.h"

Hardpoint::Hardpoint(const HardpointDesc & desc)
    : Attachment(desc), m_maxAngle(desc.maxAngle)
{
}

const std::shared_ptr<Weapon> & Hardpoint::weapon() const { return m_weapon; }

void Hardpoint::setFireRequest(const glm::vec3 & direction)
{
    if (m_weapon) m_weapon->setFireRequest(direction);
}

void Hardpoint::setReferencePose(const Pose3D & referencePose)
{
    m_referencePose = referencePose;
}

void Hardpoint::setWeapon(std::shared_ptr<Weapon> weapon) { m_weapon = weapon; }

void Hardpoint::update(const UpdateFrame & updateFrame, const EquipmentUpdateContext & context)
{
    if (!m_weapon) return;
    if (!m_enabled) return;

    m_weapon->update(updateFrame, context, worldPose(), m_maxAngle);
}

void Hardpoint::onDisabled() {  }