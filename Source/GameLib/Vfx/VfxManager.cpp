#include "VfxManager.h"

#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/ECS/World.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Texture.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Texture/TextureLoader.h>

#include "ResourceManager.h"
#include "VoxelWorld.h"

VfxManager::VfxManager(
    RenderManager & renderManager, ResourceManager & resourceManager)
    : m_resourceManager(resourceManager)
{
    m_renderer = renderManager.addRenderer<VfxRenderer>();
}

VfxRenderer & VfxManager::renderer() { return *m_renderer; }

const VfxRenderer & VfxManager::renderer() const { return *m_renderer; }

VfxMeshId VfxManager::getOrCreateMeshId(ResourceId resourceId)
{
    auto iter = m_meshIdByResourceId.find((size_t)resourceId);
    if (iter == m_meshIdByResourceId.end())
    {
        const auto & mesh = m_resourceManager.mesh(resourceId);
        const auto   meshId = m_renderer->addMesh(mesh);

        bool success;
        std::tie(iter, success) =
            m_meshIdByResourceId.emplace((size_t)resourceId, meshId);
        Assert(success, "");
    }

    return iter->second;
}

VfxParticleId VfxManager::addParticle(VfxParticle particle)
{
    return m_renderer->addParticle(particle);
}

void VfxManager::removeParticle(VfxParticleId particle)
{
    m_renderer->removeParticle(particle);
}

void VfxManager::addEmitterInstance(
    std::shared_ptr<EmitterInstance> emitterInstance)
{
    const auto index = m_emitterInstances.emplace(emitterInstance);
    emitterInstance->setId({index});
}

void VfxManager::removeEmitterInstance(
    std::shared_ptr<EmitterInstance> emitterInstance)
{
    m_emitterInstances.erase(emitterInstance->id().index);
}

void VfxManager::update(float seconds)
{
    for (size_t e = 0; e < m_emitterInstances.capacity(); e++)
    {
        if (!m_emitterInstances.contains(e)) continue;

        auto & emitterInstance = m_emitterInstances[e];

        emitterInstance->update(seconds);
        emitterInstance->setBasePose(emitterInstance->targetPose());

        if (emitterInstance->isDead()) m_deadEmitterInstances.emplace_back(e);
    }

    for (const auto & index : m_deadEmitterInstances)
    {
        m_emitterInstances.erase(index);
    }
    m_deadEmitterInstances.clear();
}
