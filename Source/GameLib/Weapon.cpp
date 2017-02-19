#include "Weapon.h"

#include <glm/gtx/vector_angle.hpp>

#include <Deliberation/Core/Math/Trajetory.h>

#include "Equipment.h"

Weapon::Weapon(const WeaponConfig & config, HailstormManager & hailstormManager, VoxelObjectWorldUID creatorUID):
    m_config(config),
    m_hailstormManager(hailstormManager),
    m_creatorUID(creatorUID)
{

}

void Weapon::setFireRequest(bool active, const glm::vec3 & target)
{
    m_fireRequestActive = active;
    m_fireRequestTarget = target;
}

void Weapon::setPose(const Pose3D & pose)
{
    m_pose = pose;
}

void Weapon::update(float seconds, const EquipmentUpdateContext & context, const Pose3D & weaponPose, float maxAngle)
{
    if (!m_fireRequestActive)
    {
        m_pose = context.targetPose;
        m_cooldown = std::max(0.0f, m_cooldown - seconds);
        return;
    }

    auto timeAccumulator = 0.0f;

    auto baseMillis = (CurrentMillis() - ((TimestampMillis)std::ceil(seconds * 1000.0f)));

    while (seconds > m_cooldown)
    {
        seconds -= m_cooldown;
        timeAccumulator += m_cooldown;

        auto intermediatePose = m_pose.interpolated(context.targetPose, timeAccumulator / (seconds + timeAccumulator));

        const auto origin = intermediatePose.poseLocalToWorld(weaponPose).position();

        auto success = false;
        auto trajectory = CalculateTrajectory(origin, context.linearVelocity, 400.0f, m_fireRequestTarget, {}, success);
        const auto trajectoryLocal = intermediatePose.directionWorldToLocal(trajectory);

        auto angle = glm::angle(trajectoryLocal, glm::vec3(0.0f, 0.0f, -1.0f));

        if (success && angle <= maxAngle)
        {
            auto bullet = HailstormParticle(origin,
                                            trajectory + context.linearVelocity,
                                            100,
                                            baseMillis + ((TimestampMillis)(timeAccumulator * 1000.0f)),
                                            2000,
                                            m_config.meshID,
                                            m_creatorUID);

            m_hailstormManager.addBullet(bullet);
            m_cooldown = m_config.cooldown;
        }
        else
        {
            seconds -= 0.001f;
        }
    }

    m_pose = context.targetPose;
    m_cooldown -= seconds;
}