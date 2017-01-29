#include "Emitter.h"

#include "HailstormParticle.h"

Emitter::Emitter(HailstormManager & manager,
                 HailstormMeshID meshID,
                 std::shared_ptr<EmitterDirectionStrategy> velocity,
                 std::shared_ptr<EmitterIntensityStrategy> intensity,
                 std::shared_ptr<EmitterLifetimeStrategy> lifetime):
    m_hailstormManager(hailstormManager),
    m_meshID(meshID),
    m_velocity(velocity),
    m_intensity(intensity),
    m_lifetime(lifetime)
{
    m_countdown = m_intensity->generateInterval();
}

void Emitter::update(float seconds, const Pose3D & pose)
{
    const auto emitterMovementDirection = position - m_position;
    const auto emitterMovementVelocity = emitterMovementDirection / seconds;

    while (true)
    {
        const auto timeStep = std::min(seconds, m_countdown);
        seconds -= timeStep;
        m_countdown -= timeStep;

        if (m_countdown > 0.0f) break;

        m_position += emitterMovementVelocity * timeStep;

        auto particle = HailstormParticle(
            m_position,
            m_velocity->generateVelocity(),
            100,
            CurrentMillis(),
            m_lifetime->generateLifetime(),
            m_meshID,
            INVALID_VOXEL_OBJECT_WORLD_UID
        );

        m_hailstormManager.addParticle(particle);

        m_countdown += m_intensity->generateInterval();
    }

    m_position = position;
}