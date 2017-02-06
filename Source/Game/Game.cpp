#include <iostream>

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
#include <Voxel/VoxelRigidBodyPayload.h>
#include <Voxel/VoxelClusterPrimitiveTest.h>

#include "AimHelper.h"
#include "Emitter.h"
#include "Equipment.h"
#include "CollisionShapeTypes.h"
#include "Hardpoint.h"
#include "HailstormRenderer.h"
#include "VoxelClusterMarchingCubesTriangulation.h"
#include "VoxelClusterMarchingCubes.h"
#include "VoxelClusterContact.h"
#include "VoxelObject.h"
#include "VoxelWorld.h"
#include "VoxReader.h"
#include "PlayerInput.h"
#include "Player/PlayerFlightControl.h"
#include "Weapon.h"

using namespace deliberation;

class Game:
    public Application
{
public:
    Game():
        Application("Verse")
    {

    }

    void onStartup() override
    {

        m_voxelWorld.reset(context(), m_physicsWorld, m_camera);

        /**
         * Create Prototypes
         */
        VoxReader voxReader;
        {
            auto models = voxReader.read("Data/VoxelClusters/ship.vox");
            if (!models.empty())
            {
                m_shipData = std::make_shared<VoxelObjectVoxelData>(*m_voxelWorld, models[0].size);
                m_shipData->addVoxels(models[0].voxels);
            }
        }
        {
            auto models = voxReader.read("Data/VoxelClusters/station.vox");
            if (!models.empty())
            {
                m_stationData = std::make_shared<VoxelObjectVoxelData>(*m_voxelWorld, models[0].size);
                m_stationData->addVoxels(models[0].voxels);
            }
        }
        {
            m_blockData = std::make_shared<VoxelObjectVoxelData>(*m_voxelWorld, glm::uvec3(3,3,3));
            for (size_t z = 0; z < m_blockData->size().z; z++)
            {
                for (size_t y = 0; y < m_blockData->size().y; y++)
                {
                    for (size_t x = 0; x < m_blockData->size().x; x++)
                    {
                        m_blockData->addVoxels({Voxel({x, y, z}, glm::vec3(0.5f, 0.4f, 0.8f))});
                    }
                }
            }
        }

        /**
         * Create VoxelObjects
         */
        m_shipObject = std::make_shared<VoxelObject>(*m_shipData);
        m_shipObject->body()->transform().setPosition({0.0f, 0.0f, 35.0f});

        m_stationObject = std::make_shared<VoxelObject>(*m_stationData);
        m_stationObject->body()->transform().setPosition({-16.0f, 40.0f, -200.0f});
        m_stationObject->body()->setAngularVelocity({0.0f, 0.0f, 0.05f});

        m_blockObject = std::make_shared<VoxelObject>(*m_blockData);
        m_blockObject->body()->transform().setPosition({0.0f, 0.0f, 0.0f});

        /**
         * Add VoxelObjects to world
         */
        m_voxelWorld->addVoxelObject(m_shipObject);
        m_voxelWorld->addVoxelObject(m_stationObject);
        m_voxelWorld->addVoxelObject(m_blockObject);

        /**
         *
         */
        m_camera.setPosition({0.0f, 10.0f, 3.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)context().backbuffer().width() / context().backbuffer().height());

        m_clear = context().createClear();

        m_dolly.reset(m_camera);

        m_debugGeometryManager.reset(context());
        m_debugGeometryRenderer.reset(*m_debugGeometryManager);

        m_physicsWorld.narrowphase().contactDispatcher().
            registerContactType<VoxelClusterContact>((int)::CollisionShapeType::VoxelCluster);

        m_physicsWorld.narrowphase().registerPrimitiveTest((int)::CollisionShapeType::VoxelCluster, std::make_unique<VoxelClusterPrimitiveTest>());

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

        m_flightControl.reset(m_shipObject->body(), playerShipConfig);

        m_playerInput.reset(input(), m_camera, *m_flightControl);

        m_hailstormManager.reset(context(), m_camera, m_physicsWorld, *m_voxelWorld);

        auto bulletMesh = UVSphere(5, 5).generateMesh2();
        m_bulletMeshID = m_hailstormManager->renderer().addMesh(bulletMesh);

        m_equipment.reset();

        WeaponConfig weaponConfig;
        weaponConfig.cooldown = 1.0f / 10.0f;
        weaponConfig.meshID = m_bulletMeshID;

        {
            auto hardpointLeft = std::make_shared<Hardpoint>(Pose3D::atPosition({-10.0f, -2.0f, -8.0f}));
            auto weaponLeft = std::make_shared<Weapon>(weaponConfig, *m_hailstormManager, m_shipObject->id().worldUID);
            hardpointLeft->setWeapon(weaponLeft);
            m_equipment->addHardpoint(hardpointLeft);
        }

        {
            auto hardpointRight = std::make_shared<Hardpoint>(Pose3D::atPosition({10.0f, -2.0f, -8.0f}));
            auto weaponRight = std::make_shared<Weapon>(weaponConfig, *m_hailstormManager, m_shipObject->id().worldUID);
            hardpointRight->setWeapon(weaponRight);
            m_equipment->addHardpoint(hardpointRight);
        }
    }

    void onFrame(float seconds) override
    {
        m_playerInput->update(seconds);
        m_flightControl->update(seconds);

        auto simulatedTime = m_physicsWorld.update(seconds);
        m_hailstormManager->update(seconds);

        glm::vec3 offset;
        offset.z = m_shipObject->data().size().z * 1.4f;
        offset.y = m_shipObject->data().size().y * 2;

        m_dolly->update(m_shipObject->body()->transform().position() +
                        m_shipObject->body()->transform().orientation() * offset,
                        m_shipObject->body()->transform().orientation(), simulatedTime);

        {
            AimHelper aimHelper(m_camera, m_physicsWorld);

            auto target = aimHelper.getTarget(input().mousePosition());

            if (input().mouseButtonDown(InputBase::MouseButton_Right))
            {
                m_equipment->setFireRequest(true,
                                            target);
            }
            else
            {
                m_equipment->setFireRequest(false, {});
            }
            m_equipment->update(seconds, Pose3D(m_shipObject->body()->transform().position(),
                                                m_shipObject->body()->transform().orientation()));

        }

        m_clear.schedule();

        m_debugGeometryRenderer->schedule(m_camera);

        m_voxelWorld->update(seconds);
        m_hailstormManager->update(seconds);
    }

private:
    Clear       m_clear;
    Camera3D    m_camera;

    Optional<CameraDolly3D>
                m_dolly;

    Optional<DebugGeometryManager>
                m_debugGeometryManager;
    Optional<DebugGeometryRenderer>
                m_debugGeometryRenderer;

    PhysicsWorld
                m_physicsWorld;

    Optional<PlayerFlightControl>
                m_flightControl;

    Optional<PlayerInput>
                m_playerInput;

    Optional<VoxelWorld>
                m_voxelWorld;
    std::shared_ptr<VoxelObject>
                m_shipObject;
    std::shared_ptr<VoxelObject>
                m_stationObject;
    std::shared_ptr<VoxelObject>
                m_blockObject;
    std::shared_ptr<VoxelObjectVoxelData>
                m_shipData;
    std::shared_ptr<VoxelObjectVoxelData>
                m_stationData;
    std::shared_ptr<VoxelObjectVoxelData>
                m_blockData;

    Optional<HailstormManager>
                m_hailstormManager;

    Optional<Emitter>
                m_emitter;

    HailstormMeshID
                m_bulletMeshID;

    Optional<Equipment>
                m_equipment;
};

int main(int argc, char *argv[])
{
    return Game().run(argc, argv);
}
