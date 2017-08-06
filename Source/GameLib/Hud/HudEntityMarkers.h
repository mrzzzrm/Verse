#pragma once

#include <map>
#include <memory>

#include <Deliberation/Core/UpdateFrame.h>
#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>

#include <Deliberation/ECS/Entity.h>

#include "GameLib.h"
#include "HudEntityMarkersRenderer.h"
#include "HudLayer.h"

namespace deliberation
{
class DrawContext;
class PhysicsWorld;
class Camera3D;
} // namespace deliberation

class Hud;
class HudButton;

class HudEntityMarkers : public HudLayer
{
public:
    HudEntityMarkers(
        Hud &                hud,
        DrawContext &        context,
        const PhysicsWorld & physicsWorld,
        const Camera3D &     camera);

    void update(const UpdateFrame & updateFrame) override;
    void render() override;

private:
    const PhysicsWorld & m_physicsWorld;
    const Camera3D &     m_camera;
    size_t               m_numVisibleMarkers = 0;

    HudEntityMarkersRenderer m_renderer;

    std::vector<std::shared_ptr<HudButton>> m_visibleMarkers;

    std::map<Entity, std::shared_ptr<HudButton>> m_markersByEntity;
};