#include "LaunchDefenseBehaviour.h"

#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/World.h>

#include "Equipment.h"
#include "VerseEntityPrototypeSystem.h"

//void LaunchDefenseBehaviour::onBehaviourUpdate(const UpdateFrame & updateFrame)
//{
//    if (m_lastLaunchedMillis + 5000 > updateFrame.beginMicros()/1000) return;
//    if (m_numLaunched >= 3) return;
//
//    auto & equipment = m_entity.component<Equipment>();
//    if (equipment.dockingPoints().empty()) return;
//
//    auto & dockingPoint = equipment.dockingPoints().front();
//
//    auto launchPose = dockingPoint->worldPose();
//
//    auto & prototypeManager =
//        m_entity.world().system<VerseEntityPrototypeSystem>()->manager();
//
//    auto npcEntity = prototypeManager->createEntity(
//        {"Drone", "Npc", "Pirate"},
//        "StationDefense" + std::to_string(m_numLaunched));
//
//    npcEntity.component<Transform3DComponent>().value() =
//        Transform3D::fromPose(launchPose);
//
//    m_numLaunched++;
//    m_lastLaunchedMillis = updateFrame.beginMicros()/1000;
//};