#include "DebugOverlay.h"

#include <iostream>

#include <imgui.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Framebuffer.h>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>

#include "HailstormManager.h"

DebugOverlay::DebugOverlay(World & world, DrawContext & context):
    Base(world),
    m_application(world.systemRef<ApplicationSystem>().application())
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
    }
    ImGui::End();

    /**
     * Entity overview
     */
    if (ImGui::Begin("ECS"))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
        if (ImGui::CollapsingHeader("Entities"))
        {
            ImGui::Columns(2, "EntityColumns");

            ImGui::BeginChild("Sub1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 300), false, ImGuiWindowFlags_HorizontalScrollbar);
            for (auto & entityData : world().entities())
            {
                auto entity = world().entity(entityData.id);
                auto label = entity.name() + " - #" + std::to_string(entity.id());

                if (ImGui::Selectable(label.c_str(), m_selectedEntity == entity))
                {
                    m_selectedEntity = entity;
                }
            }
            ImGui::EndChild();

            ImGui::NextColumn();

            if (m_selectedEntity.isValid())
            {
                if (ImGui::Button("Remove")) m_selectedEntity.scheduleRemoval();
            }
            else
            {
                ImGui::Text("No entity selected");
            }
        }

        ImGui::PopStyleVar();
    }
    ImGui::End();
}