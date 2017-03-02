#pragma once

#include <memory>

#include "Emitter.h"
#include "EmitterInstance.h"
#include "GameLib.h"


class Engine final
{
public:
    Engine(std::shared_ptr<Emitter> emitter);

    const std::shared_ptr<EmitterInstance> emitterInstance() const;

private:
    std::shared_ptr<EmitterInstance> m_emitterInstance;
};