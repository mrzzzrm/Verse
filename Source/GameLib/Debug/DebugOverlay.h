#pragma once

#include <limits>

#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Font/Label.h>
#include <Deliberation/Font/LabelRenderer.h>
#include <Deliberation/Font/MultilineLabel.h>
#include <Deliberation/Font/Font.h>

#include "GameLib.h"

namespace deliberation
{
class Application;
}

class DebugOverlay:
    public System<DebugOverlay>
{
public:
    DebugOverlay(World & world, DrawContext & context);

    void setFps(float fps);

protected:
    void onFrameUpdate(float seconds) override;

private:
    Application &   m_application;
    float           m_fps;
    Entity          m_selectedEntity;
    std::pair<EntityId, ComponentTypeId> m_selectedComponent;
    size_t          m_selectedSystem = std::numeric_limits<size_t>::max();
    std::string     m_selectedEntityPrototype;
    std::string     m_selectedComponentPrototype;
};