#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include "EmitterIntensityStrategy.h"
#include "EmitterLifetimeStrategy.h"
#include "EmitterVelocityStrategy.h"
#include "GameLib.h"
#include "HailstormDefines.h"

class HailstormManager;

class Emitter final
{
public:
    Emitter(HailstormManager & hailstormManager,
            HailstormMeshID meshID,
            std::shared_ptr<EmitterVelocityStrategy> velocity,
            std::shared_ptr<EmitterIntensityStrategy> intensity,
            std::shared_ptr<EmitterLifetimeStrategy> lifetime,
            const Pose3D & pose = Pose3D());

    Pose3D & pose();

    void update(float seconds, const Pose3D & pose);

private:
    HailstormManager &  m_hailstormManager;
    HailstormMeshID     m_meshID;
    std::shared_ptr<EmitterVelocityStrategy>
                        m_velocity;
    std::shared_ptr<EmitterIntensityStrategy>
                        m_intensity;
    std::shared_ptr<EmitterLifetimeStrategy>
                        m_lifetime;
    Pose3D              m_pose;
    float               m_countdown = 0.0f;
};