#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include "GameLib.h"

class HailstormManager;

class EmitterIntensityStrategy
{
public:
    virtual ~EmitterIntensityStrategy();
    virtual float generateInterval() const;
};

class EmitterLifetimeStrategy
{
public:
    virtual ~EmitterLifetimeStrategy();
    virtual float generateLifetime() const;
};

class Emitter final
{
public:
    Emitter(HailstormManager & hailstormManager,
            HailstormMeshID meshID,
            std::shared_ptr<EmitterVelocityStrategy> velocity,
            std::shared_ptr<EmitterIntensityStrategy> intensity,
            std::shared_ptr<EmitterLifetimeStrategy> lifetime);

    void update(float seconds, const Pose3D & pose);

private:
    HailstormManager &  m_hailstormManager;
    HailstormMeshID     m_meshID;
    std::shared_ptr<EmitterDirectionStrategy>
                        m_direction;
    std::shared_ptr<EmitterIntensityStrategy>
                        m_intensity;
    Pose3D              m_pose;
    float               m_countdown = 0.0f;
};