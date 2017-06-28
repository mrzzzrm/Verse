#include "VersePrototypeManager.h"

#include <fstream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/ComponentPrototypes.h>

#include "AllegiancePrototype.h"
#include "CoriolisPrototype.h"
#include "EquipmentPrototype.h"
#include "FlightControlConfigPrototype.h"
#include "HudProxyPrototype.h"
#include "NpcControllerPrototype.h"
#include "PlayerFlightControlPrototype.h"
#include "PlayerFlightControl.h"
#include "VoxelRigidBodyPrototype.h"
#include "VfxSystem.h"
#include "VoxelObjectPrototype.h"
#include "NpcBehaviourPrototype.h"

VersePrototypeManager::VersePrototypeManager(World & world, const std::string & listPath):
    PrototypeManager(world, listPath)
{
    auto voxelWorld = m_world.system<VoxelWorld>();
    auto & vfxManager = m_world.systemRef<VfxSystem>().manager();

    registerComponentPrototype<VoxelObjectPrototype>("VoxelObject", voxelWorld);
    registerComponentPrototype<VoxelRigidBodyPrototype>("VoxelRigidBody");
    registerComponentPrototype<CoriolisPrototype>("Coriolis");
    registerComponentPrototype<EquipmentPrototype>("Equipment", vfxManager);
    registerComponentPrototype<FlightControlConfigPrototype>("FlightControlConfig");
    registerComponentPrototype<NpcControllerPrototype>("NpcController");
    registerComponentPrototype<PlayerFlightControlPrototype>("PlayerFlightControl");
    registerComponentPrototype<AllegiancePrototype>("Allegiance");
    registerComponentPrototype<NpcBehaviourPrototype>("NpcBehaviour");
    registerComponentPrototype<NpcBehaviourPrototype>("NpcBehaviour");
    registerComponentPrototype<HudProxyPrototype>("HudProxy");
    registerComponentPrototype<Transform3DComponentPrototype>("Transform3D");
}
