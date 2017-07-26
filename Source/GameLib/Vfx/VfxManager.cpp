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

VfxMeshId VfxManager::getOrCreateMeshId(const ResourceToken & resourceToken)
{
    auto iter = m_meshIdByResourceId.find((size_t)resourceToken.id());
    if (iter == m_meshIdByResourceId.end())
    {
        const auto & mesh = m_resourceManager.resource<std::shared_ptr<MeshData>>(resourceToken);
        const auto   processedMesh = processMesh(mesh);
        const auto   meshId = m_renderer->addMesh(processedMesh);

        bool success;
        std::tie(iter, success) =
            m_meshIdByResourceId.emplace((size_t)resourceToken.id(), meshId);
        Assert(success, "");
    }

    return iter->second;
}

VfxParticleId VfxManager::addParticle(const VfxParticle & particle)
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

std::shared_ptr<MeshData> VfxManager::processMesh(const std::shared_ptr<MeshData> & inputMesh)
{
    const auto & inputVertices = inputMesh->vertices();
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
            Assert(inputLayoutField.type() == Type_Vec3, "Only conversion from vec3 supported right now");

            auto inputIterator = inputVertices.citerator<glm::vec3>(inputLayoutField);
            auto outputIterator = outputVertices.iterator<glm::vec4>(outputLayout.field(f));

            for (size_t v = 0; v < inputVertices.count(); v++)
            {
                outputIterator.put(glm::vec4{inputIterator.get(), 1.0f});
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