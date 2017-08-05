#include "VfxSystem.h"

#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/Core/EventDomain.h>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/World.h>
#include <Deliberation/ECS/Components.h>

#include <Deliberation/ImGui/ImGuiSystem.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/RenderSystem.h>

#include "Emitter.h"
#include "ResourceManager.h"
#include "VfxManager.h"
#include "VoxelObject.h"
#include "VoxelObjectBulletHit.h"
#include "VoxelObjectModification.h"

VfxSystem::VfxSystem(World & world)
    : Base(world)
    , m_vfxManager(std::make_shared<VfxManager>(
          world.systemRef<RenderSystem>().renderManager(),
          world.systemRef<ResourceManager>()))
{
    m_debugRenderer = std::make_shared<VfxDebugRenderer>(world.systemRef<RenderSystem>().renderManager(),
                                                         m_vfxManager);

    subscribeEvent<VoxelObjectModification>();
}

void VfxSystem::onEvent(const VoxelObjectModification & modification)
{
//    auto & transform =
//        modification.entity.component<Transform3DComponent>().value();
//
//    for (const auto & voxel : modification.destructions)
//    {
//        const auto position = transform.pointLocalToWorld(glm::vec3(voxel));
//
//        auto emitterInstance =
//            std::make_shared<EmitterInstance>(m_smokeEmitter);
//        emitterInstance->setBasePose(Pose3D::atPosition(position));
//
//        m_vfxManager->addEmitterInstance(emitterInstance);
//    }
}

void VfxSystem::onEvent(const PrototypesReloadedEvent & event)
{
    m_vfxManager->rebuildEmitterInstances();
}

void VfxSystem::onCreated()
{
    subscribeEvent<PrototypesReloadedEvent>();
}

void VfxSystem::onGameUpdate(float seconds) {
    m_vfxManager->update(seconds);

    auto & imGuiSystem = world().systemRef<ImGuiSystem>();
    if (!imGuiSystem.showView("Debug VFX"))
    {
        m_debugRenderer->setVisible(false);
        return;
    }

    m_debugRenderer->setVisible(true);
    m_debugRenderer->update();
}
