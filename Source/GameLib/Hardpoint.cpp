#include "Hardpoint.h"

#include <Deliberation/Core/StreamUtils.h>

#include "Weapon.h"
#include "Equipment.h"

Hardpoint::Hardpoint(const HardpointDesc & desc):
    ItemSlot(desc),
    m_maxAngle(desc.maxAngle)
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

    if (m_enabled)
    {
        auto voxelPose = context.targetPose.poseLocalToWorld(Pose3D::atPosition(glm::vec3(m_desc.voxel)));
        auto hardpointTargetPose = voxelPose.poseLocalToWorld(m_desc.pose);

        m_weapon->update(seconds, context, voxelPose, m_maxAngle);
    }
}

void Hardpoint::onDisabled()
{
    std::cout << "Hardpoint disabled" << std::endl;
}