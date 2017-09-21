#include "DebugOverlay.h"

#include <iostream>

#include <imgui.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Framebuffer.h>

#include <Deliberation/ECS/ComponentPrototype.h>
#include <Deliberation/ECS/EntityPrototypeManager.h>

#include <Deliberation/ImGui/ImGuiSystem.h>

#include <Deliberation/Platform/Application.h>
#include <Deliberation/Platform/ApplicationRuntime.h>

#include "HailstormManager.h"

DebugOverlay::DebugOverlay(World & world, DrawContext & context)
    : Base(world)
{
    m_selectedComponent = std::make_pair(ECS_INVALID_ENTITY_ID, 0);

    activatePhases<FrameUpdatePhase>();
}

void DebugOverlay::setFps(float fps) { m_fps = fps; }

void DebugOverlay::onFrameUpdate(const UpdateFrame & updateFrame)
{
    auto imGuiSystem = world().system<ImGuiSystem>();

    if (!imGuiSystem) return;

    //    bool open = true;
    //    ImGui::ShowTestWindow(&open);
    m_fps = Application::get().fps();

    const auto & profiler = world().profiler();
    const auto   numScopes = std::min<size_t>(profiler.scopes().size(), 5u);

    if (imGuiSystem->showView("Performance"))
    {
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

                ImGui::BeginChild(
                    "Sub1",
                    ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 300),
                    false,
                    ImGuiWindowFlags_HorizontalScrollbar);
                for (auto & entityData : world().entities())
                {
                    auto entity = world().entity(entityData.id);
                    auto label =
                        entity.name() + " - #" + std::to_string(entity.id());

                    if (ImGui::Selectable(
                            label.c_str(), m_selectedEntity == entity))
                    {
                        m_selectedEntity = entity;
                    }
                }
                ImGui::EndChild();

                ImGui::NextColumn();

                if (m_selectedEntity.isValid())
                {
                    auto & entityData =
                        world().entityData(m_selectedEntity.id());

                    for (const auto & componentTypeId :
                         entityData.componentSetup->componentTypeIds)
                    {
                        const auto componentName =
                            world()
                                .component(entityData.id, componentTypeId)
                                ->name();
                        const auto pair =
                            std::make_pair(entityData.id, componentTypeId);

                        if (ImGui::Selectable(
                                componentName.c_str(),
                                pair == m_selectedComponent))
                        {
                            m_selectedComponent = pair;
                        }
                    }

                    ImGui::NextColumn();

                    if (m_selectedComponent.first == entityData.id)
                    {
                        auto component = world().component(
                            entityData.id, m_selectedComponent.second);
                        if (component) component->renderImGui();
                    }

                    if (ImGui::Button("Remove"))
                        m_selectedEntity.scheduleRemoval();
                }
                else
                {
                    ImGui::Text("No entity selected");
                }

                ImGui::NextColumn();
                ImGui::Columns(1);
            }

            if (ImGui::CollapsingHeader("Systems"))
            {
                ImGui::Columns(2, "System columns");

                for (const auto & pair : world().systems())
                {
                    auto & system = pair.second;
                    if (!system) continue;
                    if (ImGui::Selectable(
                            system->name().c_str(),
                            m_selectedSystem == pair.first))
                    {
                        m_selectedSystem = pair.first;
                    }
                }
                ImGui::NextColumn();

                if (world().systems().contains(m_selectedSystem))
                {
                    world().systems().at(m_selectedSystem)->renderImGui();
                }

                ImGui::NextColumn();
                ImGui::Columns(1);
            }

            if (ImGui::CollapsingHeader("Prototypes"))
            {
                ImGui::Columns(3, "Prototype columns");

                auto & prototypeManager = Application::get().runtime()->entityPrototypeManager();

                for (auto & pair : prototypeManager->entityPrototypeByKey())
                {
                    if (ImGui::Selectable(
                            pair.first.c_str(),
                            m_selectedEntityPrototype == pair.first))
                    {
                        m_selectedEntityPrototype = pair.first;
                    }
                }
                ImGui::NextColumn();

                auto entityPrototypeIter =
                    prototypeManager->entityPrototypeByKey().find(
                        m_selectedEntityPrototype);
                if (entityPrototypeIter !=
                    prototypeManager->entityPrototypeByKey().end())
                {
                    auto & entityPrototype = entityPrototypeIter->second;
                    for (const auto & componentPrototype :
                         entityPrototype->componentPrototypes())
                    {
                        if (ImGui::Selectable(
                                componentPrototype->name().c_str(),
                                m_selectedComponentPrototype ==
                                    componentPrototype->name()))
                        {
                            m_selectedComponentPrototype =
                                componentPrototype->name();
                        }
                    }
                }

                ImGui::NextColumn();
                if (entityPrototypeIter !=
                    prototypeManager->entityPrototypeByKey().end())
                {
                    auto & entityPrototype = entityPrototypeIter->second;
                    for (const auto & componentPrototype :
                         entityPrototype->componentPrototypes())
                    {
                        if (componentPrototype->name() ==
                            m_selectedComponentPrototype)
                        {
                            ImGui::TextWrapped(
                                "%s",
                                componentPrototype->json().dump().c_str());
                        }
                    }
                }

                ImGui::NextColumn();
                ImGui::Columns(1);
            }

            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    /**
     * Application Control
     */
    Application::get().setGameplayPaused(imGuiSystem->showView("Pause Gameplay"));
}