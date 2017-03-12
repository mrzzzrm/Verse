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
#include "PlayerInput.h"
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

    void onApplicationStartup() override
    {
       // deliberation::EnableGLErrorChecksAndLogging();

        /**
         * Create Prototypes
         */
        VoxReader voxReader;

        m_shipData = VoxelObjectVoxelData::fromFile(voxReader, *m_voxelWorld, "Data/VoxelClusters/ship.vox");

        {
            m_blockData = std::make_shared<VoxelObjectVoxelData>(*m_voxelWorld, glm::uvec3(3,3,3));
            for (size_t z = 0; z < m_blockData->size().z; z++)
            {
                for (size_t y = 0; y < m_blockData->size().y; y++)
                {
                    for (size_t x = 0; x < m_blockData->size().x; x++)
                    {
                        m_blockData->addVoxels({Voxel({x, y, z}, glm::vec3(0.5f, 0.4f, 0.8f), 200.0f)});
                    }
                }
            }
        }

        /**
         *
         */
        auto bulletMesh = UVSphere(5, 5).generateMesh2();
        m_bulletMeshID = m_hailstormManager->vfxManager().renderer().addMesh(bulletMesh);

        /**
         * Setup player
         */
        FlightControlConfig playerShipConfig;
        playerShipConfig.forward.acceleration = 30.0f;
        playerShipConfig.forward.maxSpeed = 100.0f;
        playerShipConfig.backward.acceleration = 20.0f;
        playerShipConfig.backward.maxSpeed = 60.0f;
        playerShipConfig.horizontal.acceleration = 20.0f;
        playerShipConfig.horizontal.maxSpeed = 60.0f;
        playerShipConfig.vertical.acceleration = 20.0f;
        playerShipConfig.vertical.maxSpeed = 60.0f;
        playerShipConfig.angular.acceleration = 3.0f;
        playerShipConfig.angular.maxSpeed = 2.0f;
    
        std::ifstream playerEquipmentPrototypeFile("Data/Prototypes/Ship.json");
        nlohmann::json obj;
        playerEquipmentPrototypeFile >> obj;

        auto playerEquipmentPrototype = std::make_shared<EquipmentPrototype>(obj["Equipment"]);

        m_player = m_world.createEntity("npc");
        auto & voxelObject = m_player.addComponent<VoxelObject>(*m_shipData);

        auto rigidBodyPayload = std::make_shared<VoxelRigidBodyPayload>(voxelObject.shared_from_this());

        m_playerBody = std::make_shared<RigidBody>(voxelObject.data().shape());
        m_playerBody->setPayload(rigidBodyPayload);
        m_playerBody->transform().setCenter(glm::vec3(voxelObject.data().size()) / 2.0f);
        m_playerBody->transform().setPosition({0.0f, 20.0f, 50.0f});

        m_player.addComponent<RigidBodyComponent>(m_playerBody);

        playerEquipmentPrototype->applyToEntity(m_player, *m_vfxManager);

        auto & equipment = m_player.component<Equipment>();
//
//        equipment.setEngine(0, std::make_shared<Engine>(m_emitterAfterburner));
//        equipment.setEngine(1, std::make_shared<Engine>(m_emitterAfterburner));

        {
            WeaponConfig weaponConfig;
            weaponConfig.cooldown = 0.2f;
            weaponConfig.meshID = m_bulletMeshID;

            auto weapon0 = std::make_shared<Weapon>(weaponConfig, *m_hailstormManager, voxelObject.id().worldUID);
            auto weapon1 = std::make_shared<Weapon>(weaponConfig, *m_hailstormManager, voxelObject.id().worldUID);

            equipment.setWeapon(0, weapon0);
            equipment.setWeapon(1, weapon1);
        }

        /**
         * Create other objects
         */
        m_flightControl.reset(m_playerBody, playerShipConfig);
        m_playerInput.reset(input(), m_camera, *m_flightControl);
        m_dolly.reset(m_camera);

        /**
         * Create station
         */
        m_station = m_entityPrototypeManager->createEntity("Station", "MyStation");
        auto stationBody = m_station.component<RigidBodyComponent>().value();
        stationBody->setTransform(Transform3D::atPosition({0.0f, 40.0f, -100.0f}));
    }

    void onApplicationUpdate(float seconds) override
    {
        m_playerInput->update(seconds);
        m_flightControl->update(seconds);

        {
            AimHelper aimHelper(m_camera, m_physicsWorld);

            auto & equipment = m_player.component<Equipment>();

            bool hit;
            auto target = aimHelper.getTarget(input().mousePosition(), hit);

            if (input().mouseButtonDown(InputBase::MouseButton_Right)) equipment.setFireRequest(true, target);
            else equipment.setFireRequest(false, {});
        }
    }

    void onApplicationPhysicsUpdate(float seconds) override
    {
        glm::vec3 offset;
        offset.z = m_shipData->size().z * 1.4f;
        offset.y = m_shipData->size().y * 2;

        Pose3D targetPose(m_playerBody->transform().position() +
                          m_playerBody->transform().orientation() * offset,
                          m_playerBody->transform().orientation());

        //auto targetPose = m_player.component<VoxelObject>().pose();

        auto position = targetPose.pointLocalToWorld(offset);

//        m_camera.setPosition(position);
//        m_camera.setOrientation(targetPose.orientation());
        m_dolly->update(position, targetPose.orientation(), seconds);
    }

private:
    Optional<CameraDolly3D>                 m_dolly;
    Optional<PlayerFlightControl>           m_flightControl;
    Optional<PlayerInput>                   m_playerInput;

    Entity                                  m_player;
    Entity                                  m_station;
    std::shared_ptr<RigidBody>              m_playerBody;

    VfxMeshId                               m_bulletMeshID;

    std::shared_ptr<VoxelObjectVoxelData>   m_shipData;
    std::shared_ptr<VoxelObjectVoxelData>   m_stationData;
    std::shared_ptr<VoxelObjectVoxelData>   m_blockData;
};

int main(int argc, char *argv[])
{
    return Game().run(argc, argv);
}
