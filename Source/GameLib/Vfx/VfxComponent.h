#pragma once

#include <memory>
#include <vector>

#include <Deliberation/ECS/Component.h>

#include "Emitter.h"
#include "GameLib.h"

class VfxComponent:
    public Component<VfxComponent>
{
DELIBERATION_COMPONENT_NAME("VfxComponent")

public:


private:
    friend class VfxComponentPrototype;

private:
    std::vector<std::shared_ptr<EmitterInstance>> m_instances;
    std::vector<std::shared_ptr<const Emitter>> m_emitters;
};