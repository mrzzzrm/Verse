#include "VerseApplication.h"

#include <Deliberation/Scene/Debug/DebugPointLightSystem.h>
#include <Deliberation/Scene/Texture/TextureLoader.h>
#include <Deliberation/Scene/Effects/BloomRenderer.h>
#include <Deliberation/Scene/Lighting/PointLightRenderer.h>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/Systems/SkyboxSystem.h>

#include <Deliberation/ImGui/ImGuiSystem.h>
#include <Voxel/VoxelPhysicsSystem.h>

#include "VersePrototypeManager.h"
#include "VersePrototypeSystem.h"
#include "EquipmentSystem.h"
#include "Hud.h"
#include "FactionManager.h"
#include "NpcBehaviourSystem.h"
#include "PlayerSystem.h"
#include "ResourceManager.h"
#include "CoriolisSystem.h"
#include "VfxSystem.h"
#include "VerseRenderManager.h"
#include "VoxelClusterSplitSystem.h"
#include "VoxelPhysicsSystem.h"

VerseApplication::VerseApplication(const std::string & name, VerseApplicationSystemInitMode systemInitMode):
    Application(name),
    m_systemInitMode(systemInitMode)
{

}

void VerseApplication::onStartup()
{
    m_physicsWorld.narrowphase().registerPrimitiveTest((int)::CollisionShapeType::VoxelCluster,
                                                       std::make_unique<VoxelClusterPrimitiveTest>());

    m_physicsWorld.narrowphase().contactDispatcher().
        registerContactType<VoxelClusterContact>((int)::CollisionShapeType::VoxelCluster);

    auto skyboxPaths = std::array<std::string, 6> {
        GameDataPath("Data/Skybox/Right.png"),
        GameDataPath("Data/Skybox/Left.png"),
        GameDataPath("Data/Skybox/Top.png"),
        GameDataPath("Data/Skybox/Bottom.png"),
        GameDataPath("Data/Skybox/Front.png"),
        GameDataPath("Data/Skybox/Back.png")
    };

    auto skyboxCubemapBinary = TextureLoader(skyboxPaths).load();
    m_skyboxCubemap = drawContext().createTexture(skyboxCubemapBinary);

    m_world.addSystem<ApplicationSystem>(*this);

    if (m_systemInitMode == VerseApplicationSystemInitMode::AllSystems)
    {
        m_world.addSystem<RenderSystem>();
        auto pointLightSystem = m_world.addSystem<PointLightSystem>();
        m_world.addSystem<DebugPointLightSystem>(pointLightSystem->pointLightRenderer());
        m_world.addSystem<SkyboxSystem>(m_skyboxCubemap);
        m_world.addSystem<ResourceManager>();
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
        m_world.addSystem<VersePrototypeSystem>();
        m_world.addSystem<VoxelPhysicsSystem>();

        auto & renderManager = m_world.systemRef<RenderSystem>().renderManager();
        renderManager.addRenderer<AmbientLightRenderer>();
        renderManager.addRenderer<BloomRenderer>();
        renderManager.addRenderer<SsaoRenderer>();
        renderManager.addRenderer<HdrRenderer>();
    }

    onApplicationStartup();
}

void VerseApplication::onFrame(float seconds)
{
    m_world.frameBegin();

    auto physicsSimulationSeconds = m_physicsWorld.nextSimulationStepSeconds(seconds);

    if (EpsilonGt(physicsSimulationSeconds, 0.0f))
    {
        m_world.prePhysicsUpdate(physicsSimulationSeconds);
        m_physicsWorld.update(seconds);

        onApplicationPhysicsUpdate(physicsSimulationSeconds);
    }
    else
    {
        m_physicsWorld.update(seconds);
    }

    m_world.update(seconds);
    onApplicationUpdate(seconds);

    m_world.systemRef<RenderSystem>().renderManager().render();

    m_world.frameComplete();
}
