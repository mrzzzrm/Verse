#include "LaunchDefenseActivity.h"

#include <Deliberation/ECS/EntityPrototypeManager.h>
#include <Deliberation/ECS/RigidBodyComponent.h>
#include <Deliberation/ECS/Transform3DComponent.h>
#include <Deliberation/ECS/World.h>

#include "Equipment.h"

void LaunchDefenseActivity::onCreated()
{
    activatePhases<GameUpdatePhase>();
}

void LaunchDefenseActivity::onGameUpdate(const UpdateFrame & updateFrame)
{
    if (!entity().isValid()) return;
    if (m_lastLaunchedMillis + 5000 > updateFrame.beginMicros()/1000) return;
    if (m_numLaunched >= 3) return;

    auto & equipment = entity().component<Equipment>();
    if (equipment.dockingPoints().empty()) return;

    auto & dockingPoint = equipment.dockingPoints().front();

    auto launchPose = dockingPoint->worldPose();

    auto prototypeManager = GetGlobal<EntityPrototypeManager>();

    auto npcEntity = prototypeManager->createEntity(
        {"Drone", "Npc", "Pirate"},
        "StationDefense" + std::to_string(m_numLaunched));

    auto & transformComponent = npcEntity.component<Transform3DComponent>();
    transformComponent.setPosition(launchPose.position());
    transformComponent.setOrientation(launchPose.orientation());

    m_numLaunched++;
    m_lastLaunchedMillis = updateFrame.beginMicros()/1000;
}

void LaunchDefenseActivity::onAttachedToEntity()
{

}