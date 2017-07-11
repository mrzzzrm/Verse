#include "DebugOverlay.h"

#include <iostream>

#include <imgui.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Framebuffer.h>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>

#include <Deliberation/ImGui/ImGuiSystem.h>

#include "HailstormManager.h"

DebugOverlay::DebugOverlay(World & world, DrawContext & context):
    Base(world),
    m_application(world.systemRef<ApplicationSystem>().application())
{
    m_selectedComponent = std::make_pair(ECS_INVALID_ENTITY_ID, 0);
}

void DebugOverlay::setFps(float fps)
{
    m_fps = fps;
}

void DebugOverlay::onFrameUpdate(float seconds)
{
    auto imGuiSystem = world().system<ImGuiSystem>();

    if (!imGuiSystem) return;

//    bool open = true;
//    ImGui::ShowTestWindow(&open);
    m_fps = m_application.fps();

    const auto & profiler = world().profiler();
    const auto numScopes = std::min<size_t>(profiler.scopes().size(), 5u);

    if (imGuiSystem->showView("Performance")) {
        if (ImGui::Begin("Performance")) {
            ImGui::Text("FPS: %s", std::to_string((int) m_fps).c_str());
            ImGui::Separator();

            for (size_t s = 0; s < numScopes; s++) {
                ImGui::Text("%s", profiler.scopes()[s].toString().c_str());
            }
        }
        ImGui::End();
    }

    /**
     * Entity overview
     */
    if (imGuiSystem->showView("ECS"))
    {
        if (ImGui::Begin("ECS"))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
            if (ImGui::CollapsingHeader("Entities"))
            {
                ImGui::Columns(3, "EntityColumns");

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
                    auto & entityData = world().entityData(m_selectedEntity.id());

                    for (const auto & componentTypeId : entityData.componentSetup->componentTypeIds)
                    {
                        const auto componentName = world().component(entityData.id, componentTypeId)->name();
                        const auto pair = std::make_pair(entityData.id, componentTypeId);

                        if (ImGui::Selectable(componentName.c_str(), pair == m_selectedComponent))
                        {
                            m_selectedComponent = pair;
                        }
                    }

                    ImGui::NextColumn();

                    if (m_selectedComponent.first == entityData.id)
                    {
                        auto component = world().component(entityData.id, m_selectedComponent.second);
                        if (component) component->renderImGui();
                    }

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

    /**
     * Application Control
     */
    auto & application = world().systemRef<ApplicationSystem>().application();
    application.setGameplayPaused(imGuiSystem->showView("Pause Gameplay"));
}