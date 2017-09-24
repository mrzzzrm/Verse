#include "Weapon.h"

#include <glm/gtx/vector_angle.hpp>

#include <Deliberation/Core/UpdateFrame.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Trajectory.h>
#include <Deliberation/Physics/RigidBody.h>
#include <Deliberation/Scene/Pipeline/RenderPhase.h>

#include "Equipment.h"
#include "HailstormBullet.h"
#include "HailstormManager.h"
#include "Deliberation/Resource/ResourceManager.h"

Weapon::Weapon(
    const std::shared_ptr<const WeaponPrototype> & prototype,
    HailstormManager &   hailstormManager)
    : m_prototype(prototype)
    , m_hailstormManager(hailstormManager)
{
}

void Weapon::setFireRequest(const glm::vec3 & direction)
{
    AssertM(EpsilonEq(glm::length2(direction), 1.0f), "Normalize direction!")

    m_fireRequestActive = true;
    m_fireRequestDirection = direction;
}

void Weapon::setPose(const Pose3D & pose) { m_pose = pose; }

void Weapon::update(
    const UpdateFrame & updateFrame,
    const EquipmentUpdateContext & context,
    const Pose3D &                 weaponPose,
    float                          maxAngle)
{
    auto seconds = updateFrame.gameSeconds();

    if (!m_fireRequestActive)
    {
        m_pose = weaponPose;
        m_cooldown = std::max(0.0f, m_cooldown - seconds);
        return;
    }

    auto timeAccumulator = 0.0f;

    auto baseMillis = updateFrame.beginMicros() / 1000;

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

            /**
             * Lazily init render batch
             */
            if (m_prototype->vfxMeshRenderBatchIndex() == INVALID_VFX_MESH_RENDER_BATCH_INDEX)
            {
                auto renderBatchKey = VfxBatchKey(m_prototype->vfxMeshId(), true, RenderPhase::Forward,
                                                  VfxParticleOrientationType::World);
                auto renderBatchIndex = m_hailstormManager.vfxManager().meshRenderer().getOrCreateBatchIndex(renderBatchKey);
                m_prototype->setVfxMeshRenderBatchIndex(renderBatchIndex);
            }

            const auto velocity = m_fireRequestDirection * m_prototype->speed() +
                                  context.body->linearVelocity();

            VfxParticle particle(
                origin,
                velocity,
                baseMillis + ((TimestampMillis)(timeAccumulator * 1000.0f)),
                static_cast<DurationMillis>(m_prototype->lifetime() * 1000));

            particle.meshRenderBatchIndex = m_prototype->vfxMeshRenderBatchIndex();
            particle.birthRGBA = glm::vec4(1.0f);
            particle.deathRGBA = glm::vec4(1.0f);

            particle.birthOrientation = GetArcQuaternion({0, 0, -1}, glm::normalize(velocity));

            particle.pointLight = VfxPointLightDesc{1000.0f};
            particle.birthScale = m_prototype->scale();
            particle.birthScale = m_prototype->scale();

            HailstormBullet bullet(particle, 50.0f, 3, context.entity.id());
            bullet.explosionEmitter = m_prototype->explosionEmitter();

            m_hailstormManager.addBullet(bullet);

            Assert(m_prototype->frequency() != 0.0f);
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