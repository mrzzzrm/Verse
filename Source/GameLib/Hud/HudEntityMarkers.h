#pragma once

#include <map>
#include <memory>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>

#include <Deliberation/ECS/Entity.h>

#include "GameLib.h"

namespace deliberation
{
class Context;
class PhysicsWorld;
class Camera3D;
}

class Hud;
class HudButton;

class HudEntityMarkers
{
public:
    HudEntityMarkers(Hud & hud, Context & context,
                     const PhysicsWorld & physicsWorld,
                     const Camera3D & camera);

    void update();
    void render();

private:
    Hud &                   m_hud;
    Context &               m_context;
    const PhysicsWorld &    m_physicsWorld;
    const Camera3D &        m_camera;
    Draw                    m_draw;
    Uniform                 m_viewportSizeUniform;
    LayoutedBlob            m_instances;
    Buffer                  m_instanceBuffer;
    size_t                  m_numVisibleMarkers = 0;


    std::map<Entity, std::shared_ptr<HudButton>>
                            m_markersByEntity;
};