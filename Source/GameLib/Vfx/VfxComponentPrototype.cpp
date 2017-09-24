#include "VfxComponentPrototype.h"

#include <Deliberation/Platform/AppRuntime.h>
#include <Deliberation/Platform/App.h>

#include <Deliberation/Resource/PrototypeManager.h>

#include "VfxManager.h"
#include "VfxSystem.h"

void VfxComponentPrototype::updateComponent(const Entity & entity, VfxComponent & vfxComponent)
{
    auto world = this->world();
    Assert(world);

    auto & prototypeManager = App::get().runtime()->prototypeManager();
    auto & vfxManager = world->systemRef<VfxSystem>().manager();

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