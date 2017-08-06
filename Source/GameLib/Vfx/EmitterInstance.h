#pragma once

#include <memory>

#include <Deliberation/Core/Math/Pose3D.h>

#include "EmitterIntensityStrategy.h"
#include "GameLib.h"

namespace deliberation
{
class UpdateFrame;
}

class Emitter;
class VfxManager;

struct EmitterInstanceContext
{
    bool                                dead = false;
    float                               countdown = 0.0f;
    std::vector<EmitterInstanceContext> children;
    std::shared_ptr<EmitterIntensityContext> intensityContext;
};

struct EmitterInstanceId
{
    size_t index = 0;
};

class EmitterInstance final
{
public:
    EmitterInstance(std::shared_ptr<const Emitter> emitter);
    ~EmitterInstance();

    const Pose3D &            basePose() const;
    const Pose3D &            targetPose() const;
    /**
     * baseVelocity will not move the Emitter, but offset the velocity of the generated particles
     */
    const glm::vec3 & velocity() const { return m_velocity; }
    const EmitterInstanceId & id() const;
    bool                      isDead() const;

    void setBasePose(const Pose3D & pose);
    void setTargetPose(const Pose3D & pose);
    void setVelocity(const glm::vec3 & velocity) { m_velocity = velocity; }
    void setId(const EmitterInstanceId & id);

    void update(VfxManager & vfxManager, const UpdateFrame & updateFrame);

    /**
     * Re-parse the emitter and build a new context tree. Will start the emitter instance anew and react on any changes
     * on the Emitter.
     */
    void restart();

    void onEmitterDied();

private:
    friend class Emitter;

private:
    std::shared_ptr<const Emitter> m_emitter;
    Pose3D                   m_basePose;
    Pose3D                   m_targetPose;
    EmitterInstanceContext   m_rootContext;
    glm::vec3                m_velocity;
    EmitterInstanceId        m_id;
    size_t                   m_numActiveEmitters = 0;
};