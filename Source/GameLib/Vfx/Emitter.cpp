#include "Emitter.h"

#include "HailstormManager.h"
#include "HailstormParticle.h"

Emitter::Emitter(HailstormManager & hailstormManager,
                 HailstormMeshID meshID,
                 std::shared_ptr<EmitterVelocityStrategy> velocity,
                 std::shared_ptr<EmitterIntensityStrategy> intensity,
                 std::shared_ptr<EmitterLifetimeStrategy> lifetime,
                 const Pose3D & pose):
    m_hailstormManager(hailstormManager),
    m_meshID(meshID),
    m_velocity(velocity),
    m_intensity(intensity),
    m_lifetime(lifetime)
{
    m_countdown = m_intensity->generateInterval();
}

const Pose3D & Emitter::pose() const
{
    return m_pose;
}

void Emitter::update(float seconds, const Pose3D & pose)
{
    auto timeAccumulator = 0.0f;

    Pose3D intermediatePose;

    while (true)
    {
        const auto timeStep = std::min(seconds - timeAccumulator, m_countdown);
        timeAccumulator += timeStep;
        m_countdown -= timeStep;

        if (m_countdown > 0.0f) break;

        intermediatePose = m_pose.interpolated(pose, timeAccumulator / seconds);

        auto particle = HailstormParticle(
            intermediatePose.position(),
            intermediatePose.orientation() * m_velocity->generateVelocity(),
            100,
            CurrentMillis(),
            (DurationMillis)(m_lifetime->generateLifetime() * 1000),
            m_meshID,
            INVALID_VOXEL_OBJECT_WORLD_UID
        );

        m_hailstormManager.addParticle(particle);

        m_countdown += m_intensity->generateInterval();
    }

    m_pose = pose;
}