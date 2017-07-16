#pragma once

#include <memory>

#include <Deliberation/Core/Math/Pose3D.h>

#include "EmitterIntensityStrategy.h"
#include "GameLib.h"

class Emitter;

struct EmitterInstanceContext
{
    bool                                dead = false;
    float                               countdown = 0.0f;
    std::vector<EmitterInstanceContext> children;
};

struct EmitterInstanceId
{
    size_t index = 0;
};

class EmitterInstance final
{
public:
    EmitterInstance(std::shared_ptr<Emitter> emitter);
    ~EmitterInstance();

    const Pose3D &            basePose() const;
    const Pose3D &            targetPose() const;
    const EmitterInstanceId & id() const;
    bool                      isDead() const;

    const std::shared_ptr<EmitterIntensityContext> & intensityContext() const;

    void setBasePose(const Pose3D & pose);
    void setTargetPose(const Pose3D & pose);
    void setId(const EmitterInstanceId & id);

    void update(float seconds);

    void onEmitterDied();

private:
    friend class Emitter;

private:
    std::shared_ptr<Emitter> m_emitter;
    Pose3D                   m_basePose;
    Pose3D                   m_targetPose;
    EmitterInstanceContext   m_rootContext;
    EmitterInstanceId        m_id;
    size_t                   m_numActiveEmitters = 0;

    std::shared_ptr<EmitterIntensityContext> m_intensityContext;
};