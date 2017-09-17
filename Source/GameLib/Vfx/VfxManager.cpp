#include "VfxManager.h"

#include <Deliberation/Core/UpdateFrame.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/Math/AABB.h>

#include <Deliberation/ECS/World.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Texture.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Texture/TextureLoader.h>
#include <Deliberation/Scene/Lighting/PointLightRenderer.h>

#include "ResourceManager.h"
#include "VoxelWorld.h"

VfxManager::VfxManager(
    RenderManager & renderManager, ResourceManager & resourceManager)
    : m_resourceManager(resourceManager)
{
    m_meshRenderer = renderManager.addRenderer<VfxMeshRenderer>();
    m_pointLightManager = std::make_shared<VfxPointLightManager>(renderManager.renderer<PointLightRenderer>());
}

VfxMeshRenderer & VfxManager::meshRenderer() { return *m_meshRenderer; }

const VfxMeshRenderer & VfxManager::meshRenderer() const { return *m_meshRenderer; }

VfxMeshId VfxManager::getOrCreateMeshId(const ResourceToken & resourceToken)
{
    auto iter = m_meshIdByResourceId.find((size_t)resourceToken.id());
    if (iter == m_meshIdByResourceId.end())
    {
        const auto & mesh = m_resourceManager.resource<std::shared_ptr<MeshData>>(resourceToken);
        auto   processedMesh = processMesh(mesh);
        centerMesh(processedMesh);
        const auto   meshId = m_meshRenderer->addMesh(processedMesh);

        bool success;
        std::tie(iter, success) =
            m_meshIdByResourceId.emplace((size_t)resourceToken.id(), meshId);
        Assert(success);
    }

    return iter->second;
}

VfxParticleId VfxManager::addParticle(const VfxParticle & particle) {
    VfxParticleId particleId;
    particleId.meshRenderBatchIndex = particle.meshRenderBatchIndex;
    particleId.meshRenderBatchSlot = m_meshRenderer->addParticle(particle);

    if (particle.pointLight) {
        particleId.particlePointLight = m_pointLightManager->addParticlePointLight(particle, *particle.pointLight);
    }

    m_deathQueue.emplace(particle.birth + particle.lifetime, particleId);

    return particleId;
}

void VfxManager::disengageParticle(const VfxParticleId &particleId)
{
    m_meshRenderer->disengageParticle(particleId);

    if (particleId.particlePointLight != INVALID_SIZE_T)
    {
        m_pointLightManager->disengageParticlePointLight(particleId.particlePointLight);
    }
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

void VfxManager::rebuildEmitterInstances()
{
    for (auto & emitterInstance : m_emitterInstances)
    {
        emitterInstance->restart();
    }
}

void VfxManager::update(const UpdateFrame & updateFrame)
{
    m_meshRenderer->setCurrentMillis(updateFrame.endMicros() / 1000);
    m_pointLightManager->update(updateFrame);

    for (size_t e = 0; e < m_emitterInstances.capacity(); e++)
    {
        if (!m_emitterInstances.contains(e)) continue;

        auto & emitterInstance = m_emitterInstances[e];

        emitterInstance->update(*this, updateFrame);
        emitterInstance->setBasePose(emitterInstance->targetPose());

        if (emitterInstance->isDead()) m_deadEmitterInstances.emplace_back(e);
    }

    for (const auto & index : m_deadEmitterInstances)
    {
        m_emitterInstances.erase(index);
    }
    m_deadEmitterInstances.clear();

    /**
     * Remove expired particles
     */
    while (!m_deathQueue.empty())
    {
        const auto & nextDeath = m_deathQueue.top();


        if (updateFrame.endMicros() / 1000 > nextDeath.timeOfDeath)
        {
            const auto & particleId = nextDeath.id;

            m_meshRenderer->removeParticle(particleId);

            if (particleId.particlePointLight != INVALID_SIZE_T)
            {
                m_pointLightManager->removeParticlePointLight(particleId.particlePointLight);
            }

            m_deathQueue.pop(); // Do this AFTER removing the particle, otherwise ref into queue will be invalid
        }
        else
        {
            break;
        }
    }
}

void VfxManager::centerMesh(std::shared_ptr<MeshData> inputMesh)
{
    auto & vertices = inputMesh->vertices();
    Assert(!vertices.empty());

    /**
     * Find bounds
     */
    auto positions = vertices.iterator<glm::vec3>("Position");
    auto firstPosition = positions.get();
    AABB bounds(firstPosition, firstPosition);
    for (size_t v = 1; v < vertices.count(); v++)
    {
        bounds.enlargeToContain(positions.get());
    }

    /**
     * Offset
     */
    auto offset = -bounds.center();
    positions = vertices.iterator<glm::vec3>("Position");
    for (size_t v = 0; v < vertices.count(); v++)
    {
        positions.put(positions.peek() + offset);
    }
}

std::shared_ptr<MeshData> VfxManager::processMesh(const std::shared_ptr<MeshData> & inputMesh)
{
    auto & inputVertices = inputMesh->vertices();
    const auto & inputLayout = inputVertices.layout();

    if (!inputLayout.hasField("Color") || inputLayout.field("Color").type() == Type_Vec4) return inputMesh;

    DataLayout outputLayout;
    for (const auto & inputField : inputLayout.fields()) {
        if (inputField.name() == "Color") outputLayout.addField(DataLayoutField::Desc("Color", Type_Vec4));
        else outputLayout.addField(inputField.desc());
    }

    LayoutedBlob outputVertices(outputLayout, inputVertices.count());

    for (size_t f = 0; f < inputLayout.fields().size(); f++)
    {
        const auto & inputLayoutField = inputLayout.field(f);

        if (inputLayoutField.name() == "Color")
        {
            AssertM(inputLayoutField.type() == Type_U8Vec4, "Only conversion from Type_U8Vec4 supported right now");

            auto inputIterator = inputVertices.citerator<glm::u8vec4>(inputLayoutField);
            auto outputIterator = outputVertices.iterator<glm::vec4>(outputLayout.field(f));

            for (size_t v = 0; v < inputVertices.count(); v++)
            {
                outputIterator.put(glm::vec4{glm::vec4(inputIterator.get()) / glm::vec4(255.0f)});
            }
        }
        else
        {
            const auto inputField = inputVertices.field(inputLayoutField);
            auto outputField = outputVertices.field(outputLayout.field(f));

            for (size_t v = 0; v < inputVertices.count(); v++)
            {
                outputField[v] = inputField[v];
            }
        }
    }

    auto outputMesh = std::make_shared<MeshData>(std::move(outputVertices), LayoutedBlob(inputMesh->indices()));

    return outputMesh;
}


VfxManager::DeathEntry::DeathEntry(TimestampMillis timeOfDeath, const VfxParticleId & id)
    : timeOfDeath(timeOfDeath), id(id)
{
}

bool VfxManager::DeathEntry::
operator<(const VfxManager::DeathEntry & rhs) const
{
    return timeOfDeath > rhs.timeOfDeath;
}