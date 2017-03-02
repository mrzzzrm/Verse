#include "EmitterInstance.h"

#include <stack>

#include "Emitter.h"

EmitterInstance::EmitterInstance(std::shared_ptr<Emitter> emitter):
    m_emitter(emitter)
{
    std::function<void(EmitterInstanceContext & context, Emitter & emitter)> buildContextTree =
        [&](EmitterInstanceContext & context, Emitter & emitter) {
            context.children.resize(emitter.children().size());
            for (size_t c = 0; c < emitter.children().size(); c++)
            {
                buildContextTree(context.children[c], *emitter.children()[c]);
            }
        };

    buildContextTree(m_rootContext, *m_emitter);
}

EmitterInstance::~EmitterInstance() = default;

const Pose3D & EmitterInstance::basePose() const
{
    return m_basePose;
}

const Pose3D & EmitterInstance::targetPose() const
{
    return m_targetPose;
}

const EmitterInstanceId & EmitterInstance::id() const
{
    return m_id;
}

void EmitterInstance::setBasePose(const Pose3D & pose)
{
    m_basePose = pose;
}

void EmitterInstance::setTargetPose(const Pose3D & pose)
{
    m_targetPose = pose;
}

void EmitterInstance::setId(const EmitterInstanceId & id)
{
    m_id = id;
}

void EmitterInstance::update(float seconds)
{
    m_emitter->updateInstance(*this, m_rootContext, seconds);
}
