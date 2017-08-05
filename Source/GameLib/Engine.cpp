#include "Engine.h"

Engine::Engine(std::shared_ptr<const Emitter> emitter)
    : m_emitterInstance(std::make_shared<EmitterInstance>(emitter))
{
}

const std::shared_ptr<EmitterInstance> Engine::emitterInstance() const
{
    return m_emitterInstance;
}
