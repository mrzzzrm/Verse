#include "VerseEntityPrototypeSystem.h"

#include <Deliberation/ImGui/ImGuiSystem.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/ECS/ComponentPrototypes.h>
#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/LevelSystem.h>

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
#include "VfxSystem.h"
#include "VoxelObjectPrototype.h"
#include "VoxelRigidBodyPrototype.h"

#include "BehaviourSystem.h"

constexpr const char * RELOAD_PROTOTYPES_CONTROL = "Reload Prototypes";

VerseEntityPrototypeSystem::VerseEntityPrototypeSystem(World & world)
    : EntityPrototypeSystem(world, GameDataPath("Data/EntityPrototypes/list.json"))
{
}

void VerseEntityPrototypeSystem::onCreated()
{
    /**
     * Register ComponentPrototypes
     */
    auto   voxelWorld = m_world.system<VoxelWorld>();
    auto & vfxManager = m_world.systemRef<VfxSystem>().manager();
    auto & behaviourManager = m_world.systemRef<BehaviourSystem>().manager();

    m_manager->registerComponentPrototype<VoxelObjectPrototype>("VoxelObject", voxelWorld);
    m_manager->registerComponentPrototype<VoxelRigidBodyPrototype>("VoxelRigidBody");
    m_manager->registerComponentPrototype<CoriolisPrototype>("Coriolis");
    m_manager->registerComponentPrototype<EquipmentPrototype>("Equipment", vfxManager);
    m_manager->registerComponentPrototype<FlightControlConfigPrototype>(
        "FlightControlConfig");
    m_manager->registerComponentPrototype<NpcControllerPrototype>("NpcController");
    m_manager->registerComponentPrototype<PlayerFlightControlPrototype>(
        "PlayerFlightControl");
    m_manager->registerComponentPrototype<AllegiancePrototype>("Allegiance");
    m_manager->registerComponentPrototype<NpcBehaviourPrototype>("NpcBehaviour");
    m_manager->registerComponentPrototype<NpcBehaviourPrototype>("NpcBehaviour");
    m_manager->registerComponentPrototype<HudProxyPrototype>("HudProxy");
    m_manager->registerComponentPrototype<Transform3DComponentPrototype>("Transform3D");
    m_manager->registerComponentPrototype<BehaviourPrototype>(
        "Behaviour", behaviourManager);
    
    auto & imGuiSystem = world().systemRef<ImGuiSystem>();
    imGuiSystem.addControlItem(RELOAD_PROTOTYPES_CONTROL, [&]() {
        auto levelSystem = world().system<LevelSystem>();
        if (levelSystem)
        {
            if (levelSystem->level()) levelSystem->level()->reload();
        }

        m_manager->reloadList();
        m_manager->updateEntities();
    });
}

void VerseEntityPrototypeSystem::onRemoved()
{
    auto & imGuiSystem = world().systemRef<ImGuiSystem>();
    imGuiSystem.removeControlItem(RELOAD_PROTOTYPES_CONTROL);
}