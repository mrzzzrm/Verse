#include "VfxSystem.h"

#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/Core/EventDomain.h>
#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/ECS/Components.h>
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
    {
        auto lifetime = std::make_shared<EmitterRandomLifetime>(0.4, 0.8f);
        auto placement = std::make_shared<EmitterFixedPlacement>();
        auto velocity = std::make_shared<EmitterAnyDirection>(80.0f, 120.0f);
        auto intensity = std::make_shared<EmitterBurstIntensity>(25, 0.0f);
        auto rotation = std::make_shared<EmitterViewBillboardStrategy>();
        auto color = std::make_shared<EmitterColorOverLifetime>(
            glm::vec4{1.0f, 0.5f, 0.0f, 0.8f},
            glm::vec4{1.0f, 1.0f, 1.0f, 0.4f});
        auto size = std::make_shared<EmitterSizeOverLifetime>(2.4f, 1.2f);

        m_blastEmitter = std::make_shared<Emitter>(
            *m_vfxManager,
            m_vfxManager->getOrCreateMeshId(R::ParticleMesh),
            velocity,
            rotation,
            placement,
            intensity,
            lifetime,
            color,
            size);
    }

    {
        auto lifetime = std::make_shared<EmitterRandomLifetime>(5.5f, 8.0f);
        auto placement =
            std::make_shared<EmitterGaussianSphericalPlacement>(1.0f, 1.0f);
        auto velocity = std::make_shared<EmitterAnyDirection>(0.2f, 0.5f);
        auto intensity = std::make_shared<EmitterBurstIntensity>(5, 0.0f);
        auto rotation = std::make_shared<EmitterViewBillboardStrategy>();
        auto color = std::make_shared<EmitterColorOverLifetime>(
            glm::vec4{0.4f, 0.4f, 0.4f, 0.4f},
            glm::vec4{0.4f, 0.4f, 0.4f, 0.0f});
        auto size = std::make_shared<EmitterSizeOverLifetime>(3.0f, 7.0f);

        m_smokeEmitter = std::make_shared<Emitter>(
            *m_vfxManager,
            m_vfxManager->getOrCreateMeshId(R::ParticleMesh),
            velocity,
            rotation,
            placement,
            intensity,
            lifetime,
            color,
            size);

        m_smokeEmitter->addChild(m_blastEmitter);
    }

    subscribeEvent<VoxelObjectModification>();
    subscribeEvent<VoxelObjectBulletHit>();
}

void VfxSystem::onEvent(const VoxelObjectModification & modification)
{
    auto & transform =
        modification.entity.component<Transform3DComponent>().value();

    for (const auto & voxel : modification.destructions)
    {
        const auto position = transform.pointLocalToWorld(glm::vec3(voxel));

        auto emitterInstance =
            std::make_shared<EmitterInstance>(m_smokeEmitter);
        emitterInstance->setBasePose(Pose3D::atPosition(position));

        m_vfxManager->addEmitterInstance(emitterInstance);
    }
}

void VfxSystem::onEvent(const VoxelObjectBulletHit & hit)
{
    const auto & transform =
        hit.entity.component<Transform3DComponent>().value();

    const auto position = transform.pointLocalToWorld(glm::vec3(hit.voxel));

    auto emitterInstance = std::make_shared<EmitterInstance>(m_smokeEmitter);
    emitterInstance->setBasePose(Pose3D::atPosition(position));

    m_vfxManager->addEmitterInstance(emitterInstance);
}

void VfxSystem::onGameUpdate(float seconds) { m_vfxManager->update(seconds); }
