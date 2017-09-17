#include "VerseApplication.h"

#include <Deliberation/Scene/Debug/DebugPointLightSystem.h>
#include <Deliberation/Scene/Effects/BloomRenderer.h>
#include <Deliberation/Scene/Lighting/PointLightRenderer.h>
#include <Deliberation/Scene/Texture/TextureLoader.h>

#include <Deliberation/ECS/LevelSystem.h>
#include <Deliberation/ECS/Phase.h>
#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/Systems/SkyboxSystem.h>

#include <Deliberation/ImGui/ImGuiSystem.h>
#include <Deliberation/Resource/PrototypeSystem.h>

#include "BehaviourSystem.h"
#include "CoriolisSystem.h"
#include "DebugAttachmentSystem.h"
#include "EquipmentSystem.h"
#include "FactionManager.h"
#include "Hud.h"
#include "HullSystem.h"
#include "NpcBehaviourSystem.h"
#include "PlayerSystem.h"
#include "ResourceManager.h"
#include "VerseEntityPrototypeSystem.h"
#include "VerseRenderManager.h"
#include "VfxSystem.h"
#include "VersePrototypeSystem.h"
#include "VerseResourceManager.h"
#include "VoxelClusterSplitSystem.h"
#include "VoxelMaterialSystem.h"
#include "VoxelPhysicsSystem.h"

VerseApplication::VerseApplication(
    const std::string & name, VerseApplicationSystemInitMode systemInitMode)
    : Application(name), m_systemInitMode(systemInitMode)
{
}

void VerseApplication::onStartup()
{
    m_physicsWorld.primitiveTester().registerPrimitiveTest(
        (int)::CollisionShapeType::VoxelCluster,
        std::make_unique<VoxelClusterPrimitiveTest>());

//    m_physicsWorld.narrowphase()
//        .contactDispatcher()
//        .registerContactType<VoxelClusterContact>(
//            (int)::CollisionShapeType::VoxelCluster);

    auto skyboxPaths =
        std::array<std::string, 6>{GameDataPath("Data/Skybox/Right.png"),
                                   GameDataPath("Data/Skybox/Left.png"),
                                   GameDataPath("Data/Skybox/Top.png"),
                                   GameDataPath("Data/Skybox/Bottom.png"),
                                   GameDataPath("Data/Skybox/Front.png"),
                                   GameDataPath("Data/Skybox/Back.png")};

    auto skyboxCubemapBinary = TextureLoader(skyboxPaths).load();
    m_skyboxCubemap = drawContext().createTexture(skyboxCubemapBinary);

    m_world.addSystem<ApplicationSystem>(*this);

    if (m_systemInitMode == VerseApplicationSystemInitMode::AllSystems)
    {
        m_world.addSystem<RenderSystem>();
        auto pointLightSystem = m_world.addSystem<PointLightSystem>();
//        m_world.addSystem<DebugPointLightSystem>(
//            pointLightSystem->pointLightRenderer());
        m_world.addSystem<SkyboxSystem>(m_skyboxCubemap);
        m_world.addSystem<VerseResourceManager>();
        m_physicsWorldSystem =
            m_world.addSystem<PhysicsWorldSystem>(m_physicsWorld);
        m_world.addSystem<VoxelClusterSplitSystem>();
        m_world.addSystem<VoxelWorld>(m_skyboxCubemap);
        m_world.addSystem<NpcControllerSystem>();
        m_world.addSystem<HailstormManager>();
        m_world.addSystem<VfxSystem>();
        m_world.addSystem<DebugOverlay>(drawContext());
        m_world.addSystem<CoriolisSystem>();
        m_world.addSystem<EquipmentSystem>();
        m_world.addSystem<PlayerSystem>();
        m_world.addSystem<FactionManager>();
        m_world.addSystem<NpcBehaviourSystem>();
        m_world.addSystem<ImGuiSystem>();
        m_world.addSystem<Hud>();
        m_world.addSystem<BehaviourSystem>();
        m_world.addSystem<VerseEntityPrototypeSystem>();
        m_world.addSystem<VoxelPhysicsSystem>();
        m_world.addSystem<DebugAttachmentSystem>();
        m_world.addSystem<HullSystem>();
        m_world.addSystem<VersePrototypeSystem>();
        m_world.addSystem<VoxelMaterialSystem>();
        m_world.addSystem<LevelSystem>(GameDataPath("Data/Levels/level0.json")); // Do this last because it adds entities

        auto & renderManager =
            m_world.systemRef<RenderSystem>().renderManager();
        renderManager.addRenderer<AmbientLightRenderer>();
        renderManager.addRenderer<PointLightRenderer>();
        renderManager.addRenderer<BloomRenderer>();
        renderManager.addRenderer<SsaoRenderer>();
        renderManager.addRenderer<HdrRenderer>();
    }

    onApplicationStartup();

    deliberation::DisableGLErrorChecks();
}

void VerseApplication::onFrame(DurationMicros micros)
{
    m_world.frameBeginPhase();

    m_updateFrame.setPhysicsSeconds(0.0f);
    m_updateFrame.setBeginMicros(m_updateFrame.beginMicros() + m_updateFrame.gameMicros());

    if (!m_gameplayPaused)
    {
        m_updateFrame.setGameMicros(micros);

        const auto physicsSeconds =
            m_physicsWorldSystem->physicsWorld().probeNextSimulationStepSeconds(m_updateFrame.gameSeconds());
        m_updateFrame.setPhysicsSeconds(physicsSeconds);

        /**
         * Only call prePhysicsUpdate() / postPhysicsUpdate if there are physics steps to be expected
         */
        if (m_updateFrame.physicsSeconds() > 0)
        {
            m_world.prePhysicsUpdatePhase(m_updateFrame);
            m_physicsWorldSystem->updatePhysics(m_updateFrame);
            m_world.postPhysicsUpdatePhase(m_updateFrame);

            onApplicationPhysicsUpdate();
        }
        else
        {
            // Won't do anything but updating the PhysicWorld's internal clock
            m_physicsWorldSystem->updatePhysics(m_updateFrame);
        }

        m_world.gameUpdatePhase(m_updateFrame);
        onApplicationUpdate();
    }
    else
    {
        m_updateFrame.setGameMicros(0);
    }

    m_world.frameUpdatePhase(m_updateFrame);
    m_world.systemRef<RenderSystem>().renderManager().render();

    m_world.frameCompletePhase(m_updateFrame);
}
