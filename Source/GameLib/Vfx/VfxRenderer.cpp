#include "VfxRenderer.h"

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/ProgramInterface.h>
#include <Deliberation/Scene/Camera3D.h>

VfxRenderer::VfxRenderer(Context & context, const Camera3D & camera):
    m_context(context),
    m_camera(camera)
{
    m_program = m_context.createProgram({"Data/Shaders/Hailstorm.vert",
                                         "Data/Shaders/Hailstorm.frag"});

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
    m_batches.emplace_back(std::make_unique<VfxRenderBatch>(*this, mesh));
    return (VfxMeshId)(m_batches.size() - 1);
}

VfxParticleId VfxRenderer::addParticle(const VfxParticle & particle)
{
    Assert(particle.meshId < m_batches.size(), "MeshID not registered");
    const auto index = m_batches[particle.meshId]->addInstance(particle);

    return {index, particle.meshId};
}

void VfxRenderer::removeParticle(const VfxParticleId & particle)
{
    Assert(particle.renderBatchIndex < m_batches.size(), "MeshID not registered");
    m_batches[particle.renderBatchIndex]->removeInstance(particle.index);
}

void VfxRenderer::render()
{
    m_viewProjectionGlobal[0] = m_camera.viewProjection();
    m_timeGlobal[0] = CurrentMillis();

    m_globalsBuffer.scheduleUpload(m_globals);

    for (auto & batch : m_batches)
    {
        batch->render();
    }
}