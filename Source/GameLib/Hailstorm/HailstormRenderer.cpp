#include "HailstormRenderer.h"

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/ProgramInterface.h>
#include <Deliberation/Scene/Camera3D.h>

HailstormRenderer::HailstormRenderer(Context & context, const Camera3D & camera):
    m_context(context),
    m_camera(camera)
{
    m_program = m_context.createProgram({"Data/Shaders/Hailstorm.vert",
                                         "Data/Shaders/Hailstorm.frag"});

    std::cout << m_program.interface().toString() << std::endl;

    auto globalsDataLayout = m_program.interface().uniformBlock("Globals").layout();

    m_globals = LayoutedBlob(globalsDataLayout, 1);
    m_viewProjectionGlobal = m_globals.field<glm::mat4>("ViewProjection");
    m_timeGlobal = m_globals.field<uint32_t>("Time");

    m_globalsBuffer = m_context.createBuffer(globalsDataLayout);
}

Context & HailstormRenderer::context() const
{
    return m_context;
}

const Program & HailstormRenderer::program()
{
    return m_program;
}

const Buffer & HailstormRenderer::globalsBuffer() const
{
    return m_globalsBuffer;
}

HailstormMeshID HailstormRenderer::addMesh(const Mesh2 & mesh)
{
    m_batches.emplace_back(std::make_unique<HailstormRenderBatch>(*this, mesh));
    return (HailstormMeshID)(m_batches.size() - 1);
}

void HailstormRenderer::addBullet(HailstormBullet & bullet)
{
    Assert(bullet.id.meshID < m_batches.size(), "MeshID not registered");
    m_batches[bullet.id.meshID]->addInstance(bullet);
}

void HailstormRenderer::removeBullet(const HailstormBulletID & bullet)
{
    Assert(bullet.meshID < m_batches.size(), "MeshID not registered");
    m_batches[bullet.meshID]->removeInstance(bullet);
}

void HailstormRenderer::update(float seconds)
{
    m_viewProjectionGlobal[0] = m_camera.viewProjection();
    m_timeGlobal[0] = CurrentMillis();

    m_globalsBuffer.scheduleUpload(m_globals);

    for (auto & batch : m_batches)
    {
        batch->update();
    }
}