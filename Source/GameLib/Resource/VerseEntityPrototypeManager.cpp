#include "VerseEntityPrototypeManager.h"

#include <fstream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/ECS/ComponentPrototypes.h>
#include <Deliberation/ECS/Components.h>

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

VerseEntityPrototypeManager::VerseEntityPrototypeManager(
    World & world, const std::string & listPath)
    : EntityPrototypeManager(world, listPath)
{
    auto   voxelWorld = m_world.system<VoxelWorld>();
    auto & vfxManager = m_world.systemRef<VfxSystem>().manager();
    auto & behaviourManager = m_world.systemRef<BehaviourSystem>().manager();

    registerComponentPrototype<VoxelObjectPrototype>("VoxelObject", voxelWorld);
    registerComponentPrototype<VoxelRigidBodyPrototype>("VoxelRigidBody");
    registerComponentPrototype<CoriolisPrototype>("Coriolis");
    registerComponentPrototype<EquipmentPrototype>("Equipment", vfxManager);
    registerComponentPrototype<FlightControlConfigPrototype>(
        "FlightControlConfig");
    registerComponentPrototype<NpcControllerPrototype>("NpcController");
    registerComponentPrototype<PlayerFlightControlPrototype>(
        "PlayerFlightControl");
    registerComponentPrototype<AllegiancePrototype>("Allegiance");
    registerComponentPrototype<NpcBehaviourPrototype>("NpcBehaviour");
    registerComponentPrototype<NpcBehaviourPrototype>("NpcBehaviour");
    registerComponentPrototype<HudProxyPrototype>("HudProxy");
    registerComponentPrototype<Transform3DComponentPrototype>("Transform3D");
    registerComponentPrototype<BehaviourPrototype>(
        "Behaviour", behaviourManager);
}
