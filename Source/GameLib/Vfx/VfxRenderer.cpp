#include "VfxRenderer.h"

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>

class VfxAlphaRenderNode : public RenderNode
{
public:
    VfxAlphaRenderNode(VfxRenderer & renderer)
        : RenderNode(renderer.renderManager()), m_renderer(renderer)
    {
    }

    void render() override
    {
        m_renderer.m_viewProjectionGlobal[0] =
            m_renderManager.mainCamera().viewProjection();
        m_renderer.m_timeGlobal[0] = CurrentMillis();

        m_renderer.m_globalsBuffer.upload(m_renderer.m_globals);

        for (auto & batch : m_renderer.m_batches)
            batch.second->render();
    }

    VfxRenderer & m_renderer;
};

VfxRenderer::VfxRenderer(RenderManager & renderManager)
    : Renderer(renderManager)
{
    auto & drawContext = renderManager.drawContext();

    m_program =
        drawContext.createProgram({GameDataPath("Data/Shaders/Particle.vert"),
                                   GameDataPath("Data/Shaders/Particle.frag")});

    auto globalsDataLayout =
        m_program.interface().uniformBlockRef("Globals").layout();

    m_globals = LayoutedBlob(globalsDataLayout, 1);
    m_viewProjectionGlobal = m_globals.field<glm::mat4>("ViewProjection");
    m_timeGlobal = m_globals.field<uint32_t>("Time");

    m_globalsBuffer = drawContext.createBuffer(globalsDataLayout);
}

const Program & VfxRenderer::program() { return m_program; }

const Buffer & VfxRenderer::globalsBuffer() const { return m_globalsBuffer; }

VfxMeshId VfxRenderer::addMesh(const MeshData & mesh)
{
    m_batches.emplace(
        batchIndex(m_meshIdCounter, VfxParticleOrientationType::World),
        std::make_unique<VfxRenderBatch>(
            *this, mesh, VfxParticleOrientationType::World));
    m_batches.emplace(
        batchIndex(m_meshIdCounter, VfxParticleOrientationType::ViewBillboard),
        std::make_unique<VfxRenderBatch>(
            *this, mesh, VfxParticleOrientationType::ViewBillboard));

    return m_meshIdCounter++;
}

VfxParticleId VfxRenderer::addParticle(const VfxParticle & particle)
{
    const auto batchIndex =
        this->batchIndex(particle.meshId, particle.orientationType);

    Assert(m_batches.contains(batchIndex), "MeshID not registered");
    const auto index = m_batches[batchIndex]->addInstance(particle);

    return {index, batchIndex};
}

void VfxRenderer::removeParticle(const VfxParticleId & particle)
{
    Assert(
        m_batches.contains(particle.renderBatchIndex), "MeshID not registered");

    m_batches[particle.renderBatchIndex]->removeInstance(particle.index);
}

void VfxRenderer::registerRenderNodes()
{
    m_renderManager.registerRenderNode(
        std::make_shared<VfxAlphaRenderNode>(*this), RenderPhase::Alpha);
}

size_t VfxRenderer::batchIndex(
    VfxMeshId meshId, VfxParticleOrientationType orientationType) const
{
    return (int)VfxParticleOrientationType::_Count_ * meshId +
           (int)orientationType;
}