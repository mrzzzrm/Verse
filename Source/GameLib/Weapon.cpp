#include "Weapon.h"

#include <glm/gtx/vector_angle.hpp>

#include <Deliberation/Core/Math/Trajectory.h>
#include <Deliberation/Scene/Pipeline/RenderPhase.h>

#include "Equipment.h"
#include "HailstormBullet.h"
#include "HailstormManager.h"
#include "ResourceManager.h"

Weapon::Weapon(
    const std::shared_ptr<const WeaponPrototype> & prototype,
    HailstormManager &   hailstormManager)
    : m_prototype(prototype)
    , m_hailstormManager(hailstormManager)
{
}

void Weapon::setFireRequest(const glm::vec3 & direction)
{
    Assert(EpsilonEq(glm::length2(direction), 1.0f), "Normalize direction!")

    m_fireRequestActive = true;
    m_fireRequestDirection = direction;
}

void Weapon::setPose(const Pose3D & pose) { m_pose = pose; }

void Weapon::update(
    float                          seconds,
    const EquipmentUpdateContext & context,
    const Pose3D &                 weaponPose,
    float                          maxAngle)
{
    if (!m_fireRequestActive)
    {
        m_pose = weaponPose;
        m_cooldown = std::max(0.0f, m_cooldown - seconds);
        return;
    }

    auto timeAccumulator = 0.0f;

    auto baseMillis =
        (CurrentMillis() - ((TimestampMillis)std::ceil(seconds * 1000.0f)));

    while (seconds > m_cooldown)
    {
        seconds -= m_cooldown;
        timeAccumulator += m_cooldown;

        auto intermediatePose = m_pose.interpolated(
            weaponPose, timeAccumulator / (seconds + timeAccumulator));

        const auto origin = intermediatePose.position();
        const auto trajectoryLocal =
            intermediatePose.directionWorldToLocal(m_fireRequestDirection);

        auto angle = glm::angle(trajectoryLocal, glm::vec3(0.0f, 0.0f, -1.0f));

        if (angle <= maxAngle && angle > 0.0f)
        {
            auto & world = m_hailstormManager.world();

            if (m_prototype->vfxRenderBatchIndex() == INVALID_VFX_MESH_RENDER_BATCH_INDEX)
            {
                auto renderBatchKey = VfxBatchKey(m_prototype->vfxMeshId(), RenderPhase::GBuffer,
                                                  VfxParticleOrientationType::World);
                auto renderBatchIndex = m_hailstormManager.vfxManager().renderer().getOrCreateBatchIndex(renderBatchKey);
                m_prototype->setVfxRenderBatchIndex(renderBatchIndex);
            }

            const auto velocity = m_fireRequestDirection * m_prototype->speed() +
                                  context.linearVelocity;

            VfxParticle particle(
                origin,
                velocity,
                baseMillis + ((TimestampMillis)(timeAccumulator * 1000.0f)),
                static_cast<DurationMillis>(m_prototype->lifetime() * 1000));

            particle.meshRenderBatchIndex = m_prototype->vfxRenderBatchIndex();
            particle.birthRGBA = glm::vec4(1.0f);
            particle.deathRGBA = glm::vec4(1.0f);

            particle.birthOrientation = intermediatePose.orientation();

            particle.pointLight = VfxPointLightDesc{10.0f};

            HailstormBullet bullet(particle, 50.0f, 3, context.entity.id());

            m_hailstormManager.addBullet(bullet);

            Assert(m_prototype->frequency() != 0.0f, "");
            m_cooldown = 1.0f / m_prototype->frequency();
        }
        else
        {
            seconds -= 0.001f;
        }
    }

    m_pose = weaponPose;
    m_cooldown -= seconds;
}