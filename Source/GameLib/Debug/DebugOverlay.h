#pragma once

#include <limits>

#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Font/Font.h>
#include <Deliberation/Font/Label.h>
#include <Deliberation/Font/LabelRenderer.h>
#include <Deliberation/Font/MultilineLabel.h>

#include "GameLib.h"

namespace deliberation
{
class App;
}

class DebugOverlay : public System<DebugOverlay>
{
public:
    DebugOverlay(World & world);

    void setFps(float fps);

    void onFrameUpdate(const UpdateFrame & updateFrame);

private:
    float                                m_fps;
    Entity                               m_selectedEntity;
    std::pair<EntityId, ComponentTypeId> m_selectedComponent;
    size_t      m_selectedSystem = std::numeric_limits<size_t>::max();
    std::string m_selectedEntityPrototype;
    std::string m_selectedComponentPrototype;
    std::string m_selectedActivity;
    std::string m_selectedSurfaceKey;
};