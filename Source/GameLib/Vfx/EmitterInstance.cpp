#include "EmitterInstance.h"

#include <stack>

#include "Emitter.h"

EmitterInstance::EmitterInstance(std::shared_ptr<const Emitter> emitter)
    : m_emitter(emitter)
{
    restart();
}

EmitterInstance::~EmitterInstance() = default;

const Pose3D & EmitterInstance::basePose() const { return m_basePose; }

const Pose3D & EmitterInstance::targetPose() const { return m_targetPose; }

const EmitterInstanceId & EmitterInstance::id() const { return m_id; }

bool EmitterInstance::isDead() const { return m_numActiveEmitters == 0; }

void EmitterInstance::setBasePose(const Pose3D & pose) { m_basePose = pose; }

void EmitterInstance::setTargetPose(const Pose3D & pose)
{
    m_targetPose = pose;
}

void EmitterInstance::setId(const EmitterInstanceId & id) { m_id = id; }

void EmitterInstance::update(VfxManager & vfxManager, const UpdateFrame & updateFrame)
{
    m_emitter->updateInstance(vfxManager, *this, m_rootContext, updateFrame);
}

void EmitterInstance::restart()
{
    m_rootContext = EmitterInstanceContext();

    m_numActiveEmitters = 0;
    std::function<void(EmitterInstanceContext & context, const Emitter & emitter)>
        buildContextTree = [&](EmitterInstanceContext & context,
                               const Emitter &                emitter) {
        m_numActiveEmitters++;
        if (emitter.intensity()) context.intensityContext = emitter.intensity()->createContext();
        context.children.resize(emitter.children().size());
        for (size_t c = 0; c < emitter.children().size(); c++)
        {
            buildContextTree(context.children[c], *emitter.children()[c]);
        }
    };

    buildContextTree(m_rootContext, *m_emitter);
}

void EmitterInstance::onEmitterDied()
{
    Assert(m_numActiveEmitters > 0, "");
    m_numActiveEmitters--;
}
