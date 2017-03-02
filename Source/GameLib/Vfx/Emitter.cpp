#include "Emitter.h"

#include "VfxManager.h"
#include "VfxParticle.h"

Emitter::Emitter(VfxManager & vfxManager,
                 VfxMeshId meshID,
                 std::shared_ptr<EmitterVelocityStrategy> velocity,
                 std::shared_ptr<EmitterRotationStrategy> rotation,
                 std::shared_ptr<EmitterPlacementStrategy> placement,
                 std::shared_ptr<EmitterIntensityStrategy> intensity,
                 std::shared_ptr<EmitterLifetimeStrategy> lifetime,
                 std::shared_ptr<EmitterColorStrategy> color,
                 std::shared_ptr<EmitterSizeStrategy> size,
                 const Pose3D & pose):
    m_vfxManager(vfxManager),
    m_meshID(meshID),
    m_velocity(velocity),
    m_rotation(rotation),
    m_placement(placement),
    m_intensity(intensity),
    m_lifetime(lifetime),
    m_color(color),
    m_size(size)
{
}

Pose3D & Emitter::pose()
{
    return m_pose;
}

void Emitter::setPose(const Pose3D & pose)
{
    m_pose = pose;
}

const std::vector<std::shared_ptr<Emitter>> & Emitter::children() const
{
    return m_children;
}

void Emitter::addChild(std::shared_ptr<Emitter> child)
{
    m_children.emplace_back(std::move(child));
}

void Emitter::updateInstance(EmitterInstance & emitterInstance, EmitterInstanceContext & context, float seconds)
{
    auto timeAccumulator = 0.0f;

    Pose3D intermediatePose;

    while (true)
    {
        const auto timeStep = std::min(seconds - timeAccumulator, context.countdown);
        timeAccumulator += timeStep;
        context.countdown -= timeStep;

        if (context.countdown > 0.0f) break;

        intermediatePose = emitterInstance.m_basePose.interpolated(emitterInstance.m_targetPose,
                                                                   timeAccumulator / seconds);

        intermediatePose = intermediatePose.poseLocalToWorld(m_pose);

        const auto position = intermediatePose.position() + m_placement->generatePosition();
        const auto birth = CurrentMillis() + (TimestampMillis)(timeAccumulator * 1000);

        auto particle = VfxParticle(
            m_meshID,
            position,
            intermediatePose.orientation() * m_velocity->generateVelocity(),
            birth,
            (DurationMillis)(m_lifetime->generateLifetime() * 1000)
        );

        particle.orientationType = m_rotation->orientationType();
        particle.birthOrientation = m_rotation->generateOrientation();

        const auto color = m_color->generate();
        particle.birthRGBA = color.birthRGBA;
        particle.deathRGBA = color.deathRGBA;

        const auto scale = m_size->generate();
        particle.birthScale = scale.birthScale;
        particle.deathScale = scale.deathScale;

        m_vfxManager.addParticle(particle);

        context.countdown += m_intensity->generateInterval();
    }

    for (size_t c = 0; c < m_children.size(); c++)
    {
        m_children[c]->updateInstance(emitterInstance, context.children[c], seconds);
    }
}