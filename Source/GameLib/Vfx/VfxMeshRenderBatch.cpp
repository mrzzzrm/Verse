#include "VfxMeshRenderBatch.h"

#include <Deliberation/Core/LayoutedBlobElement.h>
#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Texture/TextureLoader.h>

#include "VfxMeshRenderer.h"

VfxMeshRenderBatch::VfxMeshRenderBatch(
    VfxMeshRenderer &              renderer,
    const std::shared_ptr<MeshData> & meshData,
    bool dlightEnabled,
    VfxParticleOrientationType orientationType,
    RenderPhase renderPhase)
    : m_renderer(renderer), m_meshData(meshData), m_dlightEnabled(dlightEnabled),
      m_orientationType(orientationType), m_renderPhase(renderPhase)
{
    auto instanceDataLayout = DataLayout({{"Origin", Type_Vec3},
                                          {"Velocity", Type_Vec3},
                                          {"Birth", Type_U32},
                                          {"Lifetime", Type_U32},
                                          {"BirthRGBA", Type_Vec4},
                                          {"DeathRGBA", Type_Vec4},
                                          {"BirthScale", Type_Float},
                                          {"DeathScale", Type_Float},
                                          {"AxisRotation", Type_Vec4}});
    if (orientationType == VfxParticleOrientationType::World)
    {
        instanceDataLayout.addField({"BirthOrientation", Type_Mat3});
    }

    m_instances = LayoutedBlob(instanceDataLayout, 0);

    m_origins = m_instances.field<glm::vec3>("Origin");
    m_velocities = m_instances.field<glm::vec3>("Velocity");
    m_births = m_instances.field<u32>("Birth");
    m_lifetimes = m_instances.field<u32>("Lifetime");
    m_birthRGBAs = m_instances.field<glm::vec4>("BirthRGBA");
    m_deathRGBAs = m_instances.field<glm::vec4>("DeathRGBA");
    m_birthScales = m_instances.field<float>("BirthScale");
    m_deathScales = m_instances.field<float>("DeathScale");
    m_axisRotation = m_instances.field<glm::vec4>("AxisRotation");

    if (orientationType == VfxParticleOrientationType::World)
    {
        m_birthOrientations = m_instances.field<glm::mat3>("BirthOrientation");
    }

    m_instanceBuffer =
        GetGlobal<DrawContext>()->createBuffer(instanceDataLayout);
}

size_t VfxMeshRenderBatch::addInstance(const VfxParticle & particle)
{
    if (m_freeInstanceSlots.empty())
    {

        if (m_freeInstanceSlots.empty())
        {
            /**
             * Create new instances
             */
            const auto NUM_NEW_INSTANCES = 50;

            auto oldNumInstances = m_instances.count();

            const auto newSize = oldNumInstances + NUM_NEW_INSTANCES;
            m_instances.resize(newSize);
            m_slotUIDs.resize(newSize, (size_t)VfxParticleId::INVALID_UID);

            for (size_t i = oldNumInstances; i < m_instances.count(); i++)
            {
                m_freeInstanceSlots.push(i);

                m_lifetimes[i] = 0;
                m_births[i] = 0;
            }
        }
    }

    auto index = m_freeInstanceSlots.top();
    m_freeInstanceSlots.pop();

    addInstanceInSlot(particle, index);

    return index;
}

void VfxMeshRenderBatch::removeInstance(const VfxParticleId & particleId)
{
    const auto index = particleId.meshRenderBatchSlot;
    disengageInstance(particleId);
    m_freeInstanceSlots.push(index);
}

void VfxMeshRenderBatch::disengageInstance(const VfxParticleId & particleId)
{
    const auto index = particleId.meshRenderBatchSlot;

    Assert(index < m_slotUIDs.size());
    if (m_slotUIDs[index] != particleId.uid) return; // This particle was previously disengaged/removed and is now disengaged again

    Assert(index < m_instances.count());

    m_lifetimes[index] = 0;
    m_births[index] = 0;
    m_instanceBuffer.upload(m_instances); // TODO does this have to happen every time?
    m_slotUIDs[index] = VfxParticleId::INVALID_UID;
}

void VfxMeshRenderBatch::update(const UpdateFrame & updateFrame)
{
}

void VfxMeshRenderBatch::render()
{
    if (m_instances.empty())
    {
        return;
    }

    if (m_drawDirty)
    {
        m_draw = GetGlobal<DrawContext>()->createDraw(m_renderer.program(), DrawPrimitive::Triangles, "VfxMeshRenderBatch");

        const auto & vertexLayout = m_meshData->vertices().layout();
        AssertM(
            vertexLayout.hasField("UV") == !m_meshData->textures().empty(), "");

        /**
         * Texturing setup
         */
        if (!vertexLayout.hasField("UV"))
        {
            m_draw.setAttribute("UV", glm::vec2(0.0f, 0.0f));

            const auto dummyTextureBinary =
                TextureLoader({1, 1}, glm::vec3(1.0f, 1.0f, 1.0f)).load();
            const auto dummyTexture =
                GetGlobal<DrawContext>()->createTexture(dummyTextureBinary);

            m_draw.sampler("Texture").setTexture(dummyTexture);
        }
        else
        {
            m_draw.sampler("Texture").setTexture(m_meshData->textures()[0]);
        }

        /**
         * Color - either use vertex supplied color or (1,1,1,1)
         */
        if (!vertexLayout.hasField("Color"))
        {
            m_draw.setAttribute("Color", glm::vec4(1.0f));
        }

        /**
         * Normals - either use vertex supplied normal or (0,0,0)
         */
        if (!vertexLayout.hasField("Normal"))
        {
            m_draw.setAttribute("Normal", glm::vec3(1.0f));
        }


        /**
         * Orientation setup
         */
        m_viewBillboardRotation = m_draw.uniform("ViewBillboardRotation");
        auto orientationTypeUniform = m_draw.uniform("OrientationType");
        switch (m_orientationType)
        {
        case VfxParticleOrientationType::World:
            orientationTypeUniform.set((int)VfxParticleOrientationType::World);
            m_viewBillboardRotation.set(glm::mat3(1.0f)); // Dummy
            break;

        case VfxParticleOrientationType::ViewBillboard:
            orientationTypeUniform.set(
                (int)VfxParticleOrientationType::ViewBillboard);
            m_draw.setAttribute("BirthOrientation", glm::mat3(1.0f));
            break;

        default: Fail("");
        }

        m_draw.addVertices(m_meshData->vertices());
        if (!m_meshData->indices().empty()) m_draw.setIndices(m_meshData->indices());
        m_draw.addInstanceBuffer(m_instanceBuffer, 1);
        m_draw.setUniformBuffer("Globals", m_renderer.globalsBuffer());
        if (m_renderPhase == RenderPhase::Alpha)
        {
        m_draw.state().setBlendState(
            {BlendEquation::Add, BlendFactor::SourceAlpha, BlendFactor::OneMinusSourceAlpha});
        }
        auto depthWrite = m_renderPhase == RenderPhase::Alpha ? DepthWrite::Disabled : DepthWrite::Enabled;
        m_draw.state().setDepthState(
            {DepthTest::Enabled, depthWrite});
        m_draw.uniform("DLightEnabled").set(m_dlightEnabled);
        m_draw.uniform("DLightDirection").set(glm::normalize(glm::vec3(0.1f, 1.0f, 0.2f)));
        m_draw.uniform("DLightColor").set(glm::vec3(0.8, 0.7f, 0.7f));

        if (m_renderPhase == RenderPhase::Alpha || m_renderPhase == RenderPhase::Forward)
        {
            auto binding = m_draw.framebufferBinding(GetGlobal<RenderManager>()->hdrBuffer());
            binding.setMapping({"Color", "Hdr"});
            binding.setMapping({"Normal", FramebufferBinding::DISCARD_FRAGMENT_OUTPUT});
            binding.setMapping({"Position", FramebufferBinding::DISCARD_FRAGMENT_OUTPUT});
            m_draw.setFramebufferBinding(binding);
        }
        else
        {
            AssertM(m_renderPhase == RenderPhase::GBuffer, "Illegal RenderPhase " + RenderPhaseToString(m_renderPhase));
            m_draw.setFramebuffer(GetGlobal<RenderManager>()->gbuffer());

            auto binding = m_draw.framebufferBinding(GetGlobal<RenderManager>()->gbuffer());
            binding.setMapping({"Color", "Diffuse"});

            m_draw.setFramebufferBinding(binding);
        }

        m_drawDirty = true;
    }

    if (m_orientationType == VfxParticleOrientationType::ViewBillboard)
    {
        auto cameraBasis =
            GetGlobal<RenderManager>()->mainCamera().pose().basis();
        m_viewBillboardRotation.set(cameraBasis);
    }

    m_draw.render();
}

void VfxMeshRenderBatch::addInstanceInSlot(
    const VfxParticle & particle, size_t index)
{
    m_origins[index] = particle.origin;
    m_velocities[index] = particle.velocity;
    m_births[index] = particle.birth;
    m_lifetimes[index] = particle.lifetime;
    m_birthRGBAs[index] = particle.birthRGBA;
    m_deathRGBAs[index] = particle.deathRGBA;
    m_birthScales[index] = particle.birthScale;
    m_deathScales[index] = particle.deathScale;
    m_axisRotation[index] = particle.axisRotation;

    if (m_orientationType == VfxParticleOrientationType::World)
    {
        m_birthOrientations[index] = glm::mat3_cast(particle.birthOrientation);
    }

    m_instanceBuffer.upload(m_instances);
}
