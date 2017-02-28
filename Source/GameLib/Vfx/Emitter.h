#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include "EmitterColorStrategy.h"
#include "EmitterIntensityStrategy.h"
#include "EmitterLifetimeStrategy.h"
#include "EmitterPlacementStrategy.h"
#include "EmitterVelocityStrategy.h"
#include "EmitterRotationStrategy.h"
#include "EmitterSizeStrategy.h"
#include "GameLib.h"
#include "VfxDefines.h"

class VfxManager;

class Emitter final
{
public:
    Emitter(VfxManager & vfxManager,
            VfxMeshId meshID,
            std::shared_ptr<EmitterVelocityStrategy> velocity,
            std::shared_ptr<EmitterRotationStrategy> rotation,
            std::shared_ptr<EmitterPlacementStrategy> placement,
            std::shared_ptr<EmitterIntensityStrategy> intensity,
            std::shared_ptr<EmitterLifetimeStrategy> lifetime,
            std::shared_ptr<EmitterColorStrategy> color,
            std::shared_ptr<EmitterSizeStrategy> size,
            const Pose3D & pose = Pose3D());

    Pose3D & pose();

    void setPose(const Pose3D & pose);

    void update(float seconds, const Pose3D & pose);

private:
    VfxManager &        m_vfxManager;
    VfxMeshId           m_meshID;
    std::shared_ptr<EmitterVelocityStrategy>
                        m_velocity;
    std::shared_ptr<EmitterRotationStrategy>
                        m_rotation;
    std::shared_ptr<EmitterPlacementStrategy>
                        m_placement;
    std::shared_ptr<EmitterIntensityStrategy>
                        m_intensity;
    std::shared_ptr<EmitterLifetimeStrategy>
                        m_lifetime;
    std::shared_ptr<EmitterColorStrategy>
                        m_color;
    std::shared_ptr<EmitterSizeStrategy>
                        m_size;

    Pose3D              m_pose;
    float               m_countdown = 0.0f;
};