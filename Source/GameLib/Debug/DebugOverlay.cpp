#include "DebugOverlay.h"

#include <iostream>

#include <imgui.h>

#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Framebuffer.h>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>

#include "HailstormManager.h"

DebugOverlay::DebugOverlay(World & world, Context & context):
    Base(world),
    m_application(world.system<ApplicationSystem>().application())
{
}

void DebugOverlay::setFps(float fps)
{
    m_fps = fps;
}

void DebugOverlay::onUpdate(float seconds)
{
//    bool open = true;
//    ImGui::ShowTestWindow(&open);
    m_fps = m_application.fps();

    const auto & profiler = world().profiler();
    const auto numScopes = std::min<size_t>(profiler.scopes().size(), 5u);

    if (ImGui::Begin("Performance"))
    {
        ImGui::Text("FPS: %s", std::to_string((int)m_fps).c_str());
        ImGui::Separator();

        for (size_t s = 0; s < numScopes; s++)
        {
            ImGui::Text("%s", profiler.scopes()[s].toString().c_str());
        }

        ImGui::End();
    }
}