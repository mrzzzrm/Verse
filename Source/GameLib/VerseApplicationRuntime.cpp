#include "VerseApplicationRuntime.h"

#include <Deliberation/Core/Assert.h>

#include <Deliberation/ECS/ComponentPrototypes.h>
#include <Deliberation/ECS/LevelSystem.h>
#include <Deliberation/ECS/Phase.h>
#include <Deliberation/ECS/Systems/SkyboxSystem.h>
#include <Deliberation/ECS/Components.h>

#include <Deliberation/ImGui/ImGuiSystem.h>
#include <Deliberation/ImGui/ImGuiSystem.h>

#include <Deliberation/Resource/PrototypeSystem.h>

#include <Deliberation/Scene/Debug/DebugPointLightSystem.h>
#include <Deliberation/Scene/Effects/BloomRenderer.h>
#include <Deliberation/Scene/Lighting/PointLightRenderer.h>
#include <Deliberation/Scene/Texture/TextureLoader.h>

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
#include "VerseRenderManager.h"
#include "VfxSystem.h"
#include "VersePrototypeSystem.h"
#include "VerseResourceManager.h"
#include "VoxelClusterSplitSystem.h"
#include "VoxelMaterialSystem.h"
#include "VoxelPhysicsSystem.h"
#include "VoxelShredderSandbox.h"
#include "AllegiancePrototype.h"
#include "BehaviourPrototype.h"
#include "CoriolisPrototype.h"
#include "EquipmentPrototype.h"
#include "FlightControlConfigPrototype.h"
#include "HudProxyPrototype.h"
#include "NpcBehaviourPrototype.h"
#include "NpcControllerPrototype.h"
#include "PlayerFlightControl.h"
#include "PlayerFlightControlPrototype.h"
#include "VfxComponentPrototype.h"
#include "VfxSystem.h"
#include "VoxelObjectPrototype.h"
#include "VoxelRigidBodyPrototype.h"
#include "BehaviourSystem.h"
#include "VersePrototypeSystem.h"
#include "VoxelMaterialComponentPrototype.h"

constexpr const char * RELOAD_PROTOTYPES_CONTROL = "Reload Scene";

VerseApplicationRuntime::VerseApplicationRuntime(
VerseApplicationSystemInitMode systemInitMode)
    : ApplicationRuntime("Verse", "."), m_systemInitMode(systemInitMode)
{
}

void VerseApplicationRuntime::onStartup()
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
    m_skyboxCubemap = Application::instance().drawContext().createTexture(skyboxCubemapBinary);

    if (m_systemInitMode == VerseApplicationSystemInitMode::AllSystems)
    {
        /**
         * Register Buildin Activities
         */
        m_world->activityManager()->addActivityType<VoxelShredderSandbox>("VoxelShredderSandbox");

        /**
         * Add systems
         */
        m_world->addSystem<RenderSystem>();
        auto pointLightSystem = m_world->addSystem<PointLightSystem>();
//        m_world->addSystem<DebugPointLightSystem>(
//            pointLightSystem->pointLightRenderer());
        m_world->addSystem<SkyboxSystem>(m_skyboxCubemap);
        m_world->addSystem<VerseResourceManager>();
        m_physicsWorldSystem =
            m_world->addSystem<PhysicsWorldSystem>(m_physicsWorld);
        m_world->addSystem<VoxelClusterSplitSystem>();
        m_world->addSystem<VoxelWorld>(m_skyboxCubemap);
        m_world->addSystem<NpcControllerSystem>();
        m_world->addSystem<HailstormManager>();
        m_world->addSystem<VfxSystem>();
        m_world->addSystem<DebugOverlay>(Application::instance().drawContext());
        m_world->addSystem<CoriolisSystem>();
        m_world->addSystem<EquipmentSystem>();
        m_world->addSystem<PlayerSystem>();
        m_world->addSystem<FactionManager>();
        m_world->addSystem<NpcBehaviourSystem>();
        m_world->addSystem<ImGuiSystem>();
        m_world->addSystem<Hud>();
        m_world->addSystem<BehaviourSystem>();
        m_world->addSystem<VoxelPhysicsSystem>();
        m_world->addSystem<DebugAttachmentSystem>();
        m_world->addSystem<HullSystem>();
        m_world->addSystem<VersePrototypeSystem>();
        m_world->addSystem<VoxelMaterialSystem>();

        /**
         * Register ComponentPrototypes
         */
        {
            auto   voxelWorld = m_world->system<VoxelWorld>();
            auto & vfxManager = m_world->systemRef<VfxSystem>().manager();
            auto & behaviourManager = m_world->systemRef<BehaviourSystem>().manager();
            
            auto & manager = *m_entityPrototypeManager;
            
            manager.registerComponentPrototype<VoxelObjectPrototype>("VoxelObject", voxelWorld);
            manager.registerComponentPrototype<VoxelRigidBodyPrototype>("VoxelRigidBody");
            manager.registerComponentPrototype<CoriolisPrototype>("Coriolis");
            manager.registerComponentPrototype<EquipmentPrototype>("Equipment", vfxManager);
            manager.registerComponentPrototype<FlightControlConfigPrototype>(
            "FlightControlConfig");
            manager.registerComponentPrototype<NpcControllerPrototype>("NpcController");
            manager.registerComponentPrototype<PlayerFlightControlPrototype>(
            "PlayerFlightControl");
            manager.registerComponentPrototype<AllegiancePrototype>("Allegiance");
            manager.registerComponentPrototype<NpcBehaviourPrototype>("NpcBehaviour");
            manager.registerComponentPrototype<NpcBehaviourPrototype>("NpcBehaviour");
            manager.registerComponentPrototype<HudProxyPrototype>("HudProxy");
            manager.registerComponentPrototype<Transform3DComponentPrototype>("Transform3D");
            manager.registerComponentPrototype<BehaviourPrototype>(
            "Behaviour", behaviourManager);
            manager.registerComponentPrototype<VoxelMaterialComponentPrototype>(
            "VoxelMaterial");
            manager.registerComponentPrototype<VfxComponentPrototype>(
            "VfxComponent");
    
            auto & imGuiSystem = m_world->systemRef<ImGuiSystem>();
            imGuiSystem.addControlItem(RELOAD_PROTOTYPES_CONTROL, [&]() {
                auto prototypeSystem = m_world->system<VersePrototypeSystem>();
                if (prototypeSystem)
                {
                    prototypeSystem->reload();
                }
    
                auto levelSystem = m_world->system<LevelSystem>();
                if (levelSystem)
                {
                    if (levelSystem->level()) levelSystem->level()->reload();
                }
    
                manager.reloadList();
                manager.updateEntities();
            });

            manager.reloadList();
        }
        
        /**
         * Add Renderers
         */
        auto & renderManager =
            m_world->systemRef<RenderSystem>().renderManager();
        renderManager.addRenderer<AmbientLightRenderer>();
        renderManager.addRenderer<PointLightRenderer>();
        renderManager.addRenderer<BloomRenderer>();
        renderManager.addRenderer<SsaoRenderer>();
        renderManager.addRenderer<HdrRenderer>();

        m_world->addSystem<LevelSystem>(GameDataPath("Data/Levels/VoxelShredderSandbox.json")); // Do this last because it adds entities
    }

    onApplicationStartup();

    deliberation::DisableGLErrorChecks();
}

void VerseApplicationRuntime::onFrame(DurationMicros micros)
{
    m_world->frameBeginPhase();
    
    m_updateFrame.setPhysicsSeconds(0.0f);
    m_updateFrame.setBeginMicros(m_updateFrame.beginMicros() + m_updateFrame.gameMicros());

    if (!Application::instance().gameplayPaused())
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
            m_world->prePhysicsUpdatePhase(m_updateFrame);
            m_physicsWorldSystem->updatePhysics(m_updateFrame);
            m_world->postPhysicsUpdatePhase(m_updateFrame);

            onApplicationPhysicsUpdate();
        }
        else
        {
            // Won't do anything but updating the PhysicWorld's internal clock
            m_physicsWorldSystem->updatePhysics(m_updateFrame);
        }

        m_world->gameUpdatePhase(m_updateFrame);
        onApplicationUpdate();
    }
    else
    {
        m_updateFrame.setGameMicros(0);
    }

    m_world->frameUpdatePhase(m_updateFrame);
    m_world->systemRef<RenderSystem>().renderManager().render();

    m_world->frameCompletePhase(m_updateFrame);
}
