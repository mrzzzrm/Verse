#include "DebugOverlay.h"

#include <iostream>

#include <imgui.h>

#include <bundled/ostream.h>

#include <Deliberation/Core/ScopeStack.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/Framebuffer.h>

#include <Deliberation/ECS/ActivityManager.h>
#include <Deliberation/ECS/ComponentPrototype.h>
#include <Deliberation/ECS/EntityPrototypeManager.h>

#include <Deliberation/ImGui/ImGuiSystem.h>

#include <Deliberation/Platform/App.h>
#include <Deliberation/Platform/AppRuntime.h>

#include <Deliberation/Scene/Debug/DebugSurfaceOverlayRenderer.h>

#include "HailstormManager.h"

DebugOverlay::DebugOverlay(World & world)
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
    m_fps = GetGlobal<App>()->fps();

    if (imGuiSystem->showView("Performance"))
    {
        const auto scopes = GetGlobal<ScopeStack>();


        if (ImGui::Begin("Performance"))
        {
            ImGui::Columns(2);

            for (const auto & pair : scopes->scopesById())
            {
                ImGui::Text("%s", pair.second->name().c_str());
                ImGui::NextColumn();

                ImGui::Text("%.2f..%.2f..%.2f ms", pair.second->minDuration() / 1000.0f,
                            pair.second->medianDuration() / 1000.0f,
                            pair.second->maxDuration() / 1000.0f);
                ImGui::NextColumn();
            }

            ImGui::Columns(1);
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
                            system->name(),
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

                auto prototypeManager = GetGlobal<EntityPrototypeManager>();

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


            if (ImGui::CollapsingHeader("Activities"))
            {
                ImGui::Columns(2, "Activity columns");
                auto & activityManager = GetGlobal<World>()->activityManager();

                for (auto & activity : activityManager->activities())
                {
                    auto label = activity->name() + "#" + std::to_string((uintptr_t)activity.get());
                    if (ImGui::Selectable(label.c_str()) || m_selectedActivity == label)
                    {
                        m_selectedActivity = label;

                        ImGui::NextColumn();

                        for (const auto & phaseInvoker : activity->phaseInvokers())
                        {
                            ImGui::Text("Phase Invoker ID: %u", phaseInvoker->phaseTypeId());
                        }

                        ImGui::NextColumn();
                    }
                }

                ImGui::Columns(1);
            }

            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    /**
     * Renderers
     */
    if (imGuiSystem->showView("Rendering"))
    {
        if (ImGui::Begin("Rendering"))
        {
            if (ImGui::CollapsingHeader("Surfaces"))
            {
                if (ImGui::RadioButton("Default", m_selectedSurfaceKey.empty()))
                {
                    GetGlobal<RenderManager>()->renderer<DebugSurfaceOverlayRenderer>()->disable();
                    m_selectedSurfaceKey.clear();
                }

                // Renderer specific framebuffers
                for (auto & framebuffer : GetGlobal<RenderManager>()->framebuffers())
                {
                    if (!framebuffer.isInitialized()) return;

                    for (auto & rt : framebuffer.colorTargets())
                    {
                        const auto surfaceKey = framebuffer.name() + rt.name;

                        if (ImGui::RadioButton(
                            (framebuffer.name() + " - " + rt.name).c_str(),
                            m_selectedSurfaceKey == surfaceKey))
                        {
                            GetGlobal<RenderManager>()->renderer<DebugSurfaceOverlayRenderer>()->showSurface(rt.surface);
                            m_selectedSurfaceKey = surfaceKey;
                        }
                    }
                }
            }

            if (ImGui::CollapsingHeader("Renderers"))
            {
                for (auto & renderer : GetGlobal<RenderManager>()->renderers())
                {
                    if (ImGui::CollapsingHeader(renderer->name().c_str()))
                    {
                        renderer->renderImGui();
                    }
                }
            }
        }
        ImGui::End();
    }

    /**
     * Application Control
     */
    GetGlobal<App>()->setGameplayPaused(imGuiSystem->showView("Pause Gameplay"));
}