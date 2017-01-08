#pragma once

#include <stack>

#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/LinearMap.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>

#include <Deliberation/Scene/Mesh2.h>

#include "HailstormBullet.h"

#include "GameLib.h"

class HailstormRenderer;

class HailstormRenderBatch
{
public:
    HailstormRenderBatch(HailstormRenderer & renderer, const Mesh2 & mesh);

    void addInstance(HailstormBullet & bullet);
    void removeInstance(const HailstormBulletID & bullet);

    void update();

private:
    void addInstanceInSlot(HailstormBullet & bullet, size_t index);

private:
    HailstormRenderer & m_renderer;

    Draw                m_draw;
    Buffer              m_instanceBuffer;
    Buffer              m_vertexBuffer;
    Buffer              m_indexBuffer;

    LayoutedBlob        m_instances;
    TypedBlobValueAccessor<glm::vec3>
                        m_origins;
    TypedBlobValueAccessor<glm::vec3>
                        m_velocities;
    TypedBlobValueAccessor<u32>
                        m_lifetimes;
    TypedBlobValueAccessor<u32>
                        m_births;

    std::stack<size_t>  m_freeInstanceSlots;

    LinearMap<size_t>   m_instanceIndexByBulletID;
};