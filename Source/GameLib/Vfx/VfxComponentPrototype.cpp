#include "VfxComponentPrototype.h"

#include <Deliberation/Resource/PrototypeSystem.h>

#include "VfxManager.h"
#include "VfxSystem.h"

void VfxComponentPrototype::updateComponent(VfxComponent & vfxComponent)
{
    auto & prototypeManager = world().systemRef<PrototypeSystem>().manager();
    auto & vfxManager = world().systemRef<VfxSystem>().manager();

    for (auto & instance : vfxComponent.m_instances)
    {
        vfxManager->removeEmitterInstance(instance);
    }

    vfxComponent.m_emitters.clear();
    vfxComponent.m_instances.clear();

    for (const auto & emitterName : m_newJson["Emitters"])
    {
        auto emitter = prototypeManager->getOrCreatePrototype<Emitter>(emitterName);
        vfxComponent.m_emitters.emplace_back(emitter);

        auto emitterInstance = std::make_shared<EmitterInstance>(emitter);
        vfxComponent.m_instances.emplace_back(emitterInstance);

        vfxManager->addEmitterInstance(emitterInstance);
    }
}