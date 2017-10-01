#include "VerseRuntime.h"

#include <Deliberation/Core/Assert.h>

#include <Deliberation/ECS/ActivityComponentPrototype.h>
#include <Deliberation/ECS/ComponentPrototypes.h>
#include <Deliberation/ECS/LevelSystem.h>
#include <Deliberation/ECS/Phase.h>
#include <Deliberation/ECS/Systems/SkyboxSystem.h>
#include <Deliberation/ECS/RigidBodyComponent.h>

#include <Deliberation/ImGui/ImGuiSystem.h>
#include <Deliberation/ImGui/ImGuiSystem.h>

#include <Deliberation/Scene/Debug/DebugSurfaceOverlayRenderer.h>
#include <Deliberation/Scene/Debug/DebugPointLightSystem.h>
#include <Deliberation/Scene/Effects/BloomRenderer.h>
#include <Deliberation/Scene/SsaoRenderer.h>
#include <Deliberation/Scene/HdrRenderer.h>
#include <Deliberation/Scene/Lighting/AmbientLightRenderer.h>
#include <Deliberation/Scene/Lighting/PointLightRenderer.h>
#include <Deliberation/Scene/Texture/TextureLoader.h>
#include <Deliberation/Resource/PrototypeManager.h>
#include <Deliberation/Resource/ResourceManager.h>

#include "LaunchDefenseActivity.h"
#include "CoriolisSystem.h"
#include "DebugAttachmentSystem.h"
#include "EquipmentSystem.h"
#include "FactionManager.h"
#include "Hud.h"
#include "HullSystem.h"
#include "HullComponent.h"
#include "NpcBehaviourSystem.h"
#include "PlayerSystem.h"
#include "VfxSystem.h"
#include "VoxelClusterSplitSystem.h"
#include "VoxelMaterialSystem.h"
#include "VoxelPhysicsSystem.h"
#include "VoxelShredderSandbox.h"
#include "AllegiancePrototype.h"
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
#include "VoxelMaterialComponentPrototype.h"
#include "VoxelMeshLoader.h"

constexpr const char * RELOAD_PROTOTYPES_CONTROL = "Reload Scene";

VerseRuntime::VerseRuntime(
VerseApplicationSystemInitMode systemInitMode)
    : AppRuntime(GameDataPath("Data/EntityPrototypes/list.json")), m_systemInitMode(systemInitMode)
{
}

void VerseRuntime::onStartup()
{
    InitGlobal<RenderManager>();


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
    m_skyboxCubemap = GetGlobal<DrawContext>()->createTexture(skyboxCubemapBinary);

    if (m_systemInitMode == VerseApplicationSystemInitMode::AllSystems)
    {
        /**
         * Add Renderers
         */
        GetGlobal<RenderManager>()->addRenderer<AmbientLightRenderer>();
        GetGlobal<RenderManager>()->addRenderer<PointLightRenderer>();
        GetGlobal<RenderManager>()->addRenderer<BloomRenderer>();
        GetGlobal<RenderManager>()->addRenderer<SsaoRenderer>();
        GetGlobal<RenderManager>()->addRenderer<HdrRenderer>();
        GetGlobal<RenderManager>()->addRenderer<DebugGeometryRenderer>();
        GetGlobal<RenderManager>()->addRenderer<DebugSurfaceOverlayRenderer>();

        /**
         * Register Buildin Activities
         */
        GetGlobal<World>()->activityManager()->addActivityType<VoxelShredderSandbox>("VoxelShredderSandbox");
        GetGlobal<World>()->activityManager()->addActivityType<LaunchDefenseActivity>("LaunchDefense");

        /**
         * Register Resource Loaders
         */
        {
            auto manager = GetGlobal<ResourceManager>();

            manager->setLoader<std::shared_ptr<MeshData>>([&] (const auto & path) {
                if (StringEndsWith(path, ".vox")) {
                    return VoxelMeshLoader().load(path);
                } else {
                    Fail("Only voxel meshes supported right now, can't load '" + path + "'");
                    return std::shared_ptr<MeshData>();
                }
            });
            manager->setLoader<VoxReader::VoxelModels>([&] (const std::string & path) -> VoxReader::VoxelModels{
                auto models = VoxReader().read(path);
                Log->info("Loaded {} models", models.size());
                return models;
            });

            manager->registerBuildIns();
        }
        /**
         * Add Systems
         */
        auto pointLightSystem = GetGlobal<World>()->addSystem<PointLightSystem>();
//        GetGlobal<World>()->addSystem<DebugPointLightSystem>(
//            pointLightSystem->pointLightRenderer());
        GetGlobal<World>()->addSystem<SkyboxSystem>(m_skyboxCubemap);
        m_physicsWorldSystem =
            GetGlobal<World>()->addSystem<PhysicsWorldSystem>();
        GetGlobal<World>()->addSystem<VoxelClusterSplitSystem>();
        GetGlobal<World>()->addSystem<VoxelWorld>(m_skyboxCubemap);
        GetGlobal<World>()->addSystem<NpcControllerSystem>();
        GetGlobal<World>()->addSystem<HailstormManager>();
        GetGlobal<World>()->addSystem<VfxSystem>();
        GetGlobal<World>()->addSystem<DebugOverlay>();
        GetGlobal<World>()->addSystem<CoriolisSystem>();
        GetGlobal<World>()->addSystem<EquipmentSystem>();
        GetGlobal<World>()->addSystem<PlayerSystem>();
        GetGlobal<World>()->addSystem<FactionManager>();
        GetGlobal<World>()->addSystem<NpcBehaviourSystem>();
        GetGlobal<World>()->addSystem<ImGuiSystem>();
        GetGlobal<World>()->addSystem<Hud>();
        GetGlobal<World>()->addSystem<VoxelPhysicsSystem>();
        GetGlobal<World>()->addSystem<DebugAttachmentSystem>();
        GetGlobal<World>()->addSystem<HullSystem>();
        GetGlobal<World>()->addSystem<VoxelMaterialSystem>();

        /**
         * Register Prototypes
         */
        {
            auto manager = GetGlobal<PrototypeManager>();

            manager->addPath<WeaponPrototype>(GameDataPath("Data/Prototypes/Weapons.json"));
            manager->addPath<VoxelMaterialPalettePrototype>(GameDataPath("Data/Prototypes/VoxelMaterialPalettes.json"));
            manager->addPath<VoxelMaterialPrototype>(GameDataPath("Data/Prototypes/VoxelMaterials.json"));
            manager->addPath<Emitter>(GameDataPath("Data/Prototypes/Emitters.json"));

            manager->reload();
        }

        /**
         * Register ComponentPrototypes
         */
        {
            auto   voxelWorld = GetGlobal<World>()->system<VoxelWorld>();
            auto & vfxManager = GetGlobal<World>()->systemRef<VfxSystem>().manager();
            
            auto & manager = *GetGlobal<EntityPrototypeManager>();
            
            manager.registerComponentPrototype<ActivityComponentPrototype>("Activities");
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
            manager.registerComponentPrototype<VoxelMaterialComponentPrototype>(
            "VoxelMaterial");
            manager.registerComponentPrototype<VfxComponentPrototype>(
            "VfxComponent");
            manager.registerComponentPrototype<HullComponentPrototype>(
                "Hull");
    
            auto & imGuiSystem = GetGlobal<World>()->systemRef<ImGuiSystem>();
            imGuiSystem.addControlItem(RELOAD_PROTOTYPES_CONTROL, [&]() {
                GetGlobal<PrototypeManager>()->reload();
                GetGlobal<EventDomain>()->publishEvent(PrototypesReloadedEvent());

                auto levelSystem = GetGlobal<World>()->system<LevelSystem>();
                if (levelSystem)
                {
                    if (levelSystem->level()) levelSystem->level()->reload();
                }
    
                manager.reloadList();
                manager.updateEntities();
            });

            manager.reloadList();
        }


        GetGlobal<World>()->addSystem<LevelSystem>(GameDataPath("Data/Levels/level0.json")); // Do this last because it adds entities
    }

    GetGlobal<World>()->systemRef<PhysicsWorldSystem>().physicsWorld().primitiveTester().registerPrimitiveTest(
        (int)::CollisionShapeType::VoxelCluster,
        std::make_unique<VoxelClusterPrimitiveTest>());

    onApplicationStartup();

    deliberation::DisableGLErrorChecks();
}

void VerseRuntime::onFrame(DurationMicros micros)
{
    GetGlobal<World>()->frameBeginPhase();
    
    m_updateFrame.setPhysicsSeconds(0.0f);
    m_updateFrame.setBeginMicros(m_updateFrame.beginMicros() + m_updateFrame.gameMicros());

    if (!GetGlobal<App>()->gameplayPaused())
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
            GetGlobal<World>()->prePhysicsUpdatePhase(m_updateFrame);
            m_physicsWorldSystem->updatePhysics(m_updateFrame);
            GetGlobal<World>()->postPhysicsUpdatePhase(m_updateFrame);

            onApplicationPhysicsUpdate();
        }
        else
        {
            // Won't do anything but updating the PhysicWorld's internal clock
            m_physicsWorldSystem->updatePhysics(m_updateFrame);
        }

        GetGlobal<World>()->gameUpdatePhase(m_updateFrame);
        onApplicationUpdate();
    }
    else
    {
        m_updateFrame.setGameMicros(0);
    }

    GetGlobal<World>()->frameUpdatePhase(m_updateFrame);
    GetGlobal<RenderManager>()->render();

    GetGlobal<World>()->frameCompletePhase(m_updateFrame);
}
