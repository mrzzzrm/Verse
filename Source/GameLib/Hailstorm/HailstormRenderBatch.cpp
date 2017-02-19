#include "HailstormRenderBatch.h"

#include <Deliberation/Core/LayoutedBlobElement.h>

#include <Deliberation/Draw/Context.h>

#include "HailstormRenderer.h"

HailstormRenderBatch::HailstormRenderBatch(HailstormRenderer & renderer, const Mesh2 & mesh):
    m_renderer(renderer)
{
    auto instanceDataLayout = DataLayout({
                                             {"Origin", Type_Vec3},
                                             {"Velocity", Type_Vec3},
                                             {"Birth", Type_U32},
                                             {"Lifetime", Type_U32}
                                         });
    m_instances = LayoutedBlob(instanceDataLayout, 0);

    m_origins = m_instances.field<glm::vec3>("Origin");
    m_velocities = m_instances.field<glm::vec3>("Velocity");
    m_births = m_instances.field<u32>("Birth");
    m_lifetimes = m_instances.field<u32>("Lifetime");

    m_instanceBuffer = m_renderer.context().createBuffer(instanceDataLayout);

    m_draw = m_renderer.context().createDraw(renderer.program(), gl::GL_TRIANGLES);
    m_draw.addVertices(mesh.vertices());
    m_draw.setIndices(mesh.indices());
    m_draw.addInstanceBuffer(m_instanceBuffer, mesh.indices().count());
    m_draw.setUniformBuffer("Globals", m_renderer.globalsBuffer());
    m_draw.uniform("Color").set(glm::vec3(0.0f, 1.0f, 0.0f));
}

void HailstormRenderBatch::addInstance(HailstormParticle & bullet)
{
    if (m_freeInstanceSlots.empty())
    {
        const auto NUM_NEW_INSTANCES = 50;

        auto oldNumInstances = m_instances.count();

        m_instances.resize(oldNumInstances + NUM_NEW_INSTANCES);

        for (size_t i = oldNumInstances; i < m_instances.count(); i++)
        {
            m_freeInstanceSlots.push(i);

            m_lifetimes[i] = 0;
            m_births[i] = 0;
        }
    }

    auto index = m_freeInstanceSlots.top();
    m_freeInstanceSlots.pop();

    addInstanceInSlot(bullet, index);
}

void HailstormRenderBatch::removeInstance(const HailstormParticleID & bulletID)
{
    Assert(bulletID.renderBatchIndex < m_instances.count(), "");

    m_lifetimes[bulletID.renderBatchIndex] = 0;
    m_births[bulletID.renderBatchIndex] = 0;
    m_instanceBuffer.scheduleUpload(m_instances);

    m_freeInstanceSlots.push(bulletID.renderBatchIndex);
}

void HailstormRenderBatch::render()
{
    if (m_instances.empty())
    {
        return;
    }

    m_draw.schedule();
}

void HailstormRenderBatch::addInstanceInSlot(HailstormParticle & bullet, size_t index)
{
    bullet.id.renderBatchIndex = index;

    m_origins[index] = bullet.origin;
    m_velocities[index] = bullet.velocity;
    m_births[index] = bullet.birth;
    m_lifetimes[index] = bullet.lifetime;

    m_instanceBuffer.scheduleUpload(m_instances);
}