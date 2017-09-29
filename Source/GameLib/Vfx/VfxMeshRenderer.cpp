#include "VfxMeshRenderer.h"

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>

struct VfxRenderNode : public RenderNode
{
    explicit VfxRenderNode(VfxMeshRenderer & renderer)
        : RenderNode(renderer.shared_from_this())
    {
    }

    void render() override
    {
        for (auto & batch : m_batches)
        {
            batch->render();
        }
    }

    std::vector<std::shared_ptr<VfxMeshRenderBatch>> m_batches;
};

VfxMeshRenderer::VfxMeshRenderer()
    : Renderer("VfxMesh")
{
    m_program =
        GetGlobal<DrawContext>()->createProgram({GameDataPath("Data/Shaders/Particle.vert"),
                                   GameDataPath("Data/Shaders/Particle.frag")});

    auto globalsDataLayout =
        m_program.interface().uniformBlockRef("Globals").layout();

    m_globals = LayoutedBlob(globalsDataLayout, 1);
    m_viewGlobal = m_globals.field<glm::mat4>("View");
    m_projectionGlobal = m_globals.field<glm::mat4>("Projection");
    m_timeGlobal = m_globals.field<uint32_t>("Time");

    m_globalsBuffer = GetGlobal<DrawContext>()->createBuffer(globalsDataLayout);
}

const Program & VfxMeshRenderer::program() { return m_program; }

const Buffer & VfxMeshRenderer::globalsBuffer() const { return m_globalsBuffer; }

size_t VfxMeshRenderer::getOrCreateBatchIndex(const VfxBatchKey & key)
{
    auto iter = m_batchIndexByKey.find(key);
    if (iter != m_batchIndexByKey.end()) return iter->second;

    const auto batchIndex = m_batches.size();

    m_batchIndexByKey.emplace(key, batchIndex);

    /**
     * Build VfxRenderBatch
     */
    const auto meshId = std::get<0>(key);
    AssertM(meshId < m_meshData.size(), "MeshID not registered " + std::to_string(meshId));

    const auto & meshData = m_meshData[meshId];
    const auto dlightEnabled = std::get<1>(key);
    const auto renderPhase = std::get<2>(key);
    const auto orientationType = std::get<3>(key);

    auto batch = std::make_shared<VfxMeshRenderBatch>(*this, meshData, dlightEnabled, orientationType, renderPhase);

    m_batches.emplace_back(batch);

    // Add batch to respective node
    const auto iter2 = m_renderNodesByRenderPhase.find((std::underlying_type<RenderPhase>::type)renderPhase);
    AssertM(iter2 != m_renderNodesByRenderPhase.end(),
           "No such RenderNode for RenderPhase '" + RenderPhaseToString(renderPhase) + "'");

    iter2->second->m_batches.emplace_back(batch);

    return batchIndex;
}

VfxMeshId VfxMeshRenderer::addMesh(const std::shared_ptr<MeshData> & mesh)
{
    m_meshData.emplace_back(mesh);
    return m_meshData.size() - 1;
}

size_t VfxMeshRenderer::addParticle(const VfxParticle & particle)
{
    AssertM(particle.meshRenderBatchIndex < m_batches.size(),
           "Batch index out of range " + std::to_string(particle.meshRenderBatchIndex));

    return m_batches[particle.meshRenderBatchIndex]->addInstance(particle);
}

void VfxMeshRenderer::removeParticle(const VfxParticleId & particleId)
{
    AssertM(particleId.meshRenderBatchIndex < m_batches.size(), "Batch index out of range " + std::to_string(particleId.meshRenderBatchIndex));

    m_batches[particleId.meshRenderBatchIndex]->removeInstance(particleId);
}

void VfxMeshRenderer::disengageParticle(const VfxParticleId & particleId)
{
    AssertM(particleId.meshRenderBatchIndex < m_batches.size(), "Batch index out of range " + std::to_string(particleId.meshRenderBatchIndex));

    m_batches[particleId.meshRenderBatchIndex]->disengageInstance(particleId);
}

void VfxMeshRenderer::onRegisterRenderNodes()
{
    for (auto & pair : m_renderNodesByRenderPhase)
    {
        // TODO Remove cast when switching to modern stdlib
        GetGlobal<RenderManager>()->registerRenderNode(pair.second, (RenderPhase)pair.first);
    }
}

void VfxMeshRenderer::onBeforeRender()
{
    m_viewGlobal[0] =
        GetGlobal<RenderManager>()->mainCamera().view();
    m_projectionGlobal[0] =
        GetGlobal<RenderManager>()->mainCamera().projection();
    m_timeGlobal[0] = m_currentMillis;

    m_globalsBuffer.upload(m_globals);
}

void VfxMeshRenderer::onCreated()
{
    // Create RenderNodes
    // TODO() remove casts once switching to modern stdlib
    m_renderNodesByRenderPhase[(std::underlying_type<RenderPhase>::type)RenderPhase::GBuffer] =
        std::make_shared<VfxRenderNode>(*this);
    m_renderNodesByRenderPhase[(std::underlying_type<RenderPhase>::type)RenderPhase::Alpha] =
        std::make_shared<VfxRenderNode>(*this);
    m_renderNodesByRenderPhase[(std::underlying_type<RenderPhase>::type)RenderPhase::Forward] =
        std::make_shared<VfxRenderNode>(*this);
}