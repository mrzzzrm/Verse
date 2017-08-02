#include "VerseApplication.h"

#include <Deliberation/Scene/Debug/DebugPointLightSystem.h>
#include <Deliberation/Scene/Effects/BloomRenderer.h>
#include <Deliberation/Scene/Lighting/PointLightRenderer.h>
#include <Deliberation/Scene/Texture/TextureLoader.h>

#include <Deliberation/ECS/LevelSystem.h>
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
    m_physicsWorld.narrowphase().registerPrimitiveTest(
        (int)::CollisionShapeType::VoxelCluster,
        std::make_unique<VoxelClusterPrimitiveTest>());

    m_physicsWorld.narrowphase()
        .contactDispatcher()
        .registerContactType<VoxelClusterContact>(
            (int)::CollisionShapeType::VoxelCluster);

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
        m_world.addSystem<DebugPointLightSystem>(
            pointLightSystem->pointLightRenderer());
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
        m_world.addSystem<LevelSystem>(GameDataPath("Data/Levels/WreckageSandbox.json")); // Do this last because it adds entities

        auto & renderManager =
            m_world.systemRef<RenderSystem>().renderManager();
        renderManager.addRenderer<AmbientLightRenderer>();
        renderManager.addRenderer<PointLightRenderer>();
        renderManager.addRenderer<BloomRenderer>();
        renderManager.addRenderer<SsaoRenderer>();
        renderManager.addRenderer<HdrRenderer>();
    }

    onApplicationStartup();
}

void VerseApplication::onFrame(float seconds)
{
    m_world.frameBegin();

    if (!m_gameplayPaused)
    {
        auto physicsSimulationSeconds =
            m_physicsWorld.nextSimulationStepSeconds(seconds);

        if (EpsilonGt(physicsSimulationSeconds, 0.0f))
        {
            m_world.prePhysicsUpdate(physicsSimulationSeconds);
            m_physicsWorldSystem->updatePhysics(seconds);
            m_world.postPhysicsUpdate(physicsSimulationSeconds);

            onApplicationPhysicsUpdate(physicsSimulationSeconds);
        }
        else
        {
            m_physicsWorld.update(seconds);
        }

        m_world.gameUpdate(seconds);
        onApplicationUpdate(seconds);
    }

    m_world.frameUpdate(seconds);

    m_world.systemRef<RenderSystem>().renderManager().render();

    m_world.frameComplete(seconds);
}
