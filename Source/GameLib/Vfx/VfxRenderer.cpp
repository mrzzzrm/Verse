#include "VfxRenderer.h"

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/ProgramInterface.h>
#include <Deliberation/Scene/Camera3D.h>

VfxRenderer::VfxRenderer(Context & context, const Camera3D & camera):
    m_context(context),
    m_camera(camera)
{
    m_program = m_context.createProgram({GameDataPath("Data/Shaders/Particle.vert"),
                                         GameDataPath("Data/Shaders/Particle.frag")});

    auto globalsDataLayout = m_program.interface().uniformBlock("Globals").layout();

    m_globals = LayoutedBlob(globalsDataLayout, 1);
    m_viewProjectionGlobal = m_globals.field<glm::mat4>("ViewProjection");
    m_timeGlobal = m_globals.field<uint32_t>("Time");

    m_globalsBuffer = m_context.createBuffer(globalsDataLayout);
}

Context & VfxRenderer::context() const
{
    return m_context;
}

const Camera3D & VfxRenderer::camera() const
{
    return m_camera;
}

const Program & VfxRenderer::program()
{
    return m_program;
}

const Buffer & VfxRenderer::globalsBuffer() const
{
    return m_globalsBuffer;
}

VfxMeshId VfxRenderer::addMesh(const Mesh2 & mesh)
{
    m_batches.emplace(batchIndex(m_meshIdCounter, VfxParticleOrientationType::World),
                      std::make_unique<VfxRenderBatch>(*this, mesh, VfxParticleOrientationType::World));
    m_batches.emplace(batchIndex(m_meshIdCounter, VfxParticleOrientationType::ViewBillboard),
                      std::make_unique<VfxRenderBatch>(*this, mesh, VfxParticleOrientationType::ViewBillboard));

    return m_meshIdCounter++;
}

VfxParticleId VfxRenderer::addParticle(const VfxParticle & particle)
{
    const auto batchIndex = this->batchIndex(particle.meshId, particle.orientationType);

    Assert(m_batches.contains(batchIndex), "MeshID not registered");
    const auto index = m_batches[batchIndex]->addInstance(particle);

    return {index, batchIndex};
}

void VfxRenderer::removeParticle(const VfxParticleId & particle)
{
    Assert(m_batches.contains(particle.renderBatchIndex), "MeshID not registered");

    m_batches[particle.renderBatchIndex]->removeInstance(particle.index);
}

void VfxRenderer::update(float seconds)
{
}

void VfxRenderer::render()
{
    m_viewProjectionGlobal[0] = m_camera.viewProjection();
    m_timeGlobal[0] = CurrentMillis();

    m_globalsBuffer.scheduleUpload(m_globals);

    for (auto & batch : m_batches) batch.second->render();
}

size_t VfxRenderer::batchIndex(VfxMeshId meshId, VfxParticleOrientationType orientationType) const
{
    return (int)VfxParticleOrientationType::_Count_ * meshId + (int)orientationType;
}