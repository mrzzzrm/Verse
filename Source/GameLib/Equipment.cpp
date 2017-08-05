#include "Equipment.h"

#include <Deliberation/Core/Math/Trajectory.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Physics/RigidBody.h>

#include "Hardpoint.h"
#include "VfxManager.h"
#include "Weapon.h"

const std::vector<std::shared_ptr<Hardpoint>> & Equipment::hardpoints() const
{
    return m_hardpoints;
}

const std::vector<std::shared_ptr<EngineSlot>> & Equipment::engineSlots() const
{
    return m_engineSlots;
}

float Equipment::bulletSpeed() const
{
    if (m_hardpoints.empty()) return 0.0f;

    auto bulletSpeed = 0.0f;

    for (size_t h = 0; h < m_hardpoints.size(); h++)
    {
        const auto & weapon = m_hardpoints[h]->weapon();
        if (!weapon) continue;

        bulletSpeed = weapon->prototype()->speed();
        break;
    }

    return bulletSpeed;
}

void Equipment::clearFireRequests()
{
    for (auto & hardpoint : m_hardpoints)
        hardpoint->clearFireRequest();
}

void Equipment::setFireRequestDirectionForAllHardpoints(
    const glm::vec3 & direction)
{
    for (auto & hardpoint : m_hardpoints)
        hardpoint->setFireRequest(direction);
}

void Equipment::setFireRequestTargetForAllHardpoints(
    const Transform3D & equipmentTransform,
    const glm::vec3 &   equipmentVelocity,
    const glm::vec3 &   targetPosition,
    const glm::vec3 &   targetVelocity)
{
    for (auto & hardpoint : m_hardpoints)
    {
        const auto weapon = hardpoint->weapon();
        if (!weapon) continue;

        const auto hardpointPosition =
            equipmentTransform.pointLocalToWorld(glm::vec3(hardpoint->voxel()));

        const auto bulletSpeed = weapon->prototype()->speed();

        bool success;
        auto trajectory = CalculateTrajectory(
            hardpointPosition,
            equipmentVelocity,
            bulletSpeed,
            targetPosition,
            targetVelocity,
            success);

        if (success) hardpoint->setFireRequest(glm::normalize(trajectory));
    }
}

void Equipment::setWeapon(size_t slot, std::shared_ptr<Weapon> weapon)
{
    Assert(slot < m_hardpoints.size(), "");
    m_hardpoints[slot]->setWeapon(weapon);
}

void Equipment::setEngine(size_t slot, std::shared_ptr<Engine> engine)
{
    Assert(slot < m_engineSlots.size(), "");

    auto & previousEngine = m_engineSlots[slot]->engine();

    if (previousEngine)
    {
        m_vfxManager->removeEmitterInstance(previousEngine->emitterInstance());
    }

    m_engineSlots[slot]->setEngine(engine);

    if (engine)
    {
        m_vfxManager->addEmitterInstance(engine->emitterInstance());
    }
}

void Equipment::addAttachment(const std::shared_ptr<Attachment> & attachment)
{
    m_attachmentByVoxel.emplace(attachment->voxel(), attachment);
    attachment->setEntity(Entity(*(World *)m_world, entityId()));
    attachment->setEnabled(true);
    attachment->setIndex(m_attachments.size());
    m_attachments.emplace_back(attachment);
}

void Equipment::update(float seconds, const EquipmentUpdateContext & context)
{
    for (auto & hardpoint : m_hardpoints)
        hardpoint->update(seconds, context);
    for (auto & engineSlot : m_engineSlots)
        engineSlot->setTargetPose(context.targetPose);

    for (auto & pair : m_attachmentByVoxel)
    {
        pair.second->onGameUpdate(seconds, context);
    }
}
