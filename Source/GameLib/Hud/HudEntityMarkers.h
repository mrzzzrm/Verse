#pragma once

#include <map>
#include <memory>

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
    HudEntityMarkers(Hud & hud, Context & context, const PhysicsWorld & physicsWorld, const Camera3D & camera);

    void update();
    void render();

private:
    Hud &                   m_hud;
    const PhysicsWorld &    m_physicsWorld;
    const Camera3D &        m_camera;
    Draw                    m_draw;

    std::map<Entity, std::shared_ptr<HudButton>>
        m_markersByEntity;
};