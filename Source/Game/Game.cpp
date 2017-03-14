#include <iostream>

#include <Deliberation/Core/Json.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>
#include <Deliberation/Physics/Narrowphase.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/CameraDolly3D.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>
#include <Deliberation/Scene/Debug/DebugGeometryManager.h>
#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>
#include <Deliberation/Scene/UVSphere.h>

#include <Deliberation/Deliberation.h>


#include <Voxel/VoxelRigidBodyPayload.h>
#include <Voxel/VoxelClusterPrimitiveTest.h>

#include "AimHelper.h"
#include "Emitter.h"
#include "Equipment.h"
#include "EquipmentPrototype.h"
#include "EntityPrototypeManager.h"
#include "CollisionShapeTypes.h"
#include "Hardpoint.h"
#include "VfxRenderer.h"
#include "VoxelClusterMarchingCubesTriangulation.h"
#include "VoxelClusterMarchingCubes.h"
#include "VoxelClusterContact.h"
#include "VoxelObject.h"
#include "VoxelWorld.h"
#include "VoxReader.h"
#include "Player/PlayerFlightControl.h"
#include "VerseApplication.h"
#include "Weapon.h"

using namespace deliberation;

class Game:
    public VerseApplication
{
public:
    Game():
        VerseApplication("Verse")
    {

    }

    void onApplicationStartup() override {
        // deliberation::EnableGLErrorChecksAndLogging();

        /**
         *
         */
        auto bulletMesh = UVSphere(5, 5).generateMesh2();
        auto bulletMeshID = m_hailstormManager->vfxManager().renderer().addMesh(bulletMesh);

        /**
         * Create player
         */
        auto player = m_entityPrototypeManager->createEntity({"Ship", "Player"}, "PlayerShip");

        {
            auto &equipment = player.component<Equipment>();
            auto &voxelObject = player.component<VoxelObject>();

            WeaponConfig weaponConfig;
            weaponConfig.cooldown = 0.2f;
            weaponConfig.meshID = bulletMeshID;

            auto weapon0 = std::make_shared<Weapon>(weaponConfig, *m_hailstormManager, voxelObject.id().worldUID);
            auto weapon1 = std::make_shared<Weapon>(weaponConfig, *m_hailstormManager, voxelObject.id().worldUID);

            equipment.setWeapon(0, weapon0);
            equipment.setWeapon(1, weapon1);
        }
        {
            auto &playerBody = player.component<RigidBodyComponent>().value();
            playerBody->transform().setPosition({0.0f, 20.0f, 50.0f});
        }


        /**
         * Create station
         */
        auto station = m_entityPrototypeManager->createEntity({"Station"}, "MyStation");
        auto stationBody = station.component<RigidBodyComponent>().value();
        stationBody->transform().setPosition({0.0f, 40.0f, -100.0f});

        /**
         * Create asteroids
         */
        for (auto i = 0; i < 15; i++) {
            auto asteroid = m_entityPrototypeManager->createEntity({"Asteroid00"}, "MyAsteroid");
            auto asteroidBody = asteroid.component<RigidBodyComponent>().value();
            asteroidBody->transform().setPosition(RandomUnitVec3() * 1250.0f);
        }
        for (auto i = 0; i < 15; i++) {
            auto asteroid = m_entityPrototypeManager->createEntity({"Asteroid01"}, "MyAsteroid");
            auto asteroidBody = asteroid.component<RigidBodyComponent>().value();
            asteroidBody->transform().setPosition(RandomUnitVec3() * 1250.0f);
        }

        /**
         * Create enemies
         */
        for (auto i = 0; i < 4; i++) {
            auto npc = m_entityPrototypeManager->createEntity({"Drone", "Npc"}, "MyNPC");
            auto npcBody = npc.component<RigidBodyComponent>().value();
            npcBody->transform().setPosition(glm::vec3(300.0f, 0.0f, 0.0f) + RandomUnitVec3() * 1000.0f);
        }
    }
};

int main(int argc, char *argv[])
{
    return Game().run(argc, argv);
}
