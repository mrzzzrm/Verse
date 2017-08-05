#pragma once

#include <queue>
#include <stack>

#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/LinearMap.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Program.h>

#include <Deliberation/Scene/MeshData.h>

#include "VfxParticle.h"

#include "GameLib.h"

class VfxMeshRenderer;

class VfxMeshRenderBatch
{
public:
    VfxMeshRenderBatch(
        VfxMeshRenderer &              renderer,
        const std::shared_ptr<MeshData> & meshData,
        bool dlightEnabled,
        VfxParticleOrientationType orientationType,
        RenderPhase renderPhase);

    size_t addInstance(const VfxParticle & particle);
    void   removeInstance(size_t index);
    void   disengageInstance(size_t index);

    void update(float seconds);

    void render();

private:
    void addInstanceInSlot(const VfxParticle & bullet, size_t index);

private:
    VfxMeshRenderer & m_renderer;

    std::shared_ptr<MeshData>  m_meshData;
    bool                       m_dlightEnabled = false;
    VfxParticleOrientationType m_orientationType;
    RenderPhase                m_renderPhase;

    Draw   m_draw;
    bool   m_drawDirty = true;
    Buffer m_instanceBuffer;
    Buffer m_vertexBuffer;

    Uniform m_viewBillboardRotation;

    LayoutedBlob                      m_instances;
    TypedBlobValueAccessor<glm::vec3> m_origins;
    TypedBlobValueAccessor<glm::vec3> m_velocities;
    TypedBlobValueAccessor<u32>       m_lifetimes;
    TypedBlobValueAccessor<u32>       m_births;
    TypedBlobValueAccessor<glm::vec4> m_birthRGBAs;
    TypedBlobValueAccessor<glm::vec4> m_deathRGBAs;
    TypedBlobValueAccessor<glm::mat3> m_birthOrientations;
    TypedBlobValueAccessor<float>     m_birthScales;
    TypedBlobValueAccessor<float>     m_deathScales;
    TypedBlobValueAccessor<glm::vec4> m_axisRotation;

    std::stack<size_t> m_freeInstanceSlots;
};