#pragma once

#include <stack>
#include <queue>

#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/LinearMap.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Program.h>

#include <Deliberation/Scene/Mesh2.h>

#include "VfxParticle.h"

#include "GameLib.h"

class VfxRenderer;

class VfxRenderBatch
{
public:
    VfxRenderBatch(VfxRenderer & renderer, const Mesh2 & mesh, VfxParticleOrientationType orientationType);

    size_t addInstance(const VfxParticle & particle);
    void removeInstance(size_t index);

    void update(float seconds);

    void render();

private:
    struct DeathEntry
    {
        u32 timeOfDeath;
        size_t slot;

        DeathEntry(u32 timeOfDeath, size_t slot);
        bool operator<(const DeathEntry & rhs) const;
    };

private:
    void addInstanceInSlot(const VfxParticle & bullet, size_t index);

private:
    VfxRenderer &       m_renderer;

    VfxParticleOrientationType m_orientationType;

    Draw                m_draw;
    Buffer              m_instanceBuffer;
    Buffer              m_vertexBuffer;
    Buffer              m_indexBuffer;

    Uniform             m_viewBillboardRotation;

    LayoutedBlob                        m_instances;
    TypedBlobValueAccessor<glm::vec3>   m_origins;
    TypedBlobValueAccessor<glm::vec3>   m_velocities;
    TypedBlobValueAccessor<u32>         m_lifetimes;
    TypedBlobValueAccessor<u32>         m_births;
    TypedBlobValueAccessor<glm::vec4>   m_birthRGBAs;
    TypedBlobValueAccessor<glm::vec4>   m_deathRGBAs;
    TypedBlobValueAccessor<glm::vec4>   m_birthOrientations;
    TypedBlobValueAccessor<float>       m_birthScales;
    TypedBlobValueAccessor<float>       m_deathScales;

    std::stack<size_t>  m_freeInstanceSlots;

    LinearMap<size_t>   m_instanceIndexByBulletID;

    std::priority_queue<DeathEntry> m_deathQueue;
};