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

#include "HailstormRenderer.h"
#include "VoxelClusterMarchingCubesTriangulation.h"
#include "VoxelClusterMarchingCubes.h"
#include "VoxelClusterShape.h"
#include "VoxelClusterContact.h"
#include "VoxelObject.h"
#include "VoxelObjectPrototype.h"
#include "VoxelWorld.h"
#include "VoxReader.h"
#include "PlayerInput.h"
#include "FlightControl.h"
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
            auto clusters = voxReader.read(deliberation::dataPath("Data/VoxelCluster/ship.vox"));
            if (!clusters.empty())
            {
                m_shipPrototype = std::make_shared<VoxelObjectPrototype>(*m_voxelWorld, clusters[0]);
            }
        }
        {
            auto clusters = voxReader.read(deliberation::dataPath("Data/VoxelCluster/station.vox"));
            if (!clusters.empty())
            {
                m_stationPrototype = std::make_shared<VoxelObjectPrototype>(*m_voxelWorld, clusters[0]);
            }
        }
        {
            VoxelCluster<glm::vec3> blockCluster(glm::uvec3(5, 4, 3));
            for (size_t z = 0; z < blockCluster.size().z; z++)
            {
                for (size_t y = 0; y < blockCluster.size().y; y++)
                {
                    for (size_t x = 0; x < blockCluster.size().x; x++)
                    {
                        blockCluster.set({x, y, z}, glm::vec3(0.5f, 0.4f, 0.8f));
                    }
                }
            }
            m_blockPrototype = std::make_shared<VoxelObjectPrototype>(*m_voxelWorld, blockCluster);
        }

        /**
         * Create VoxelObjects
         */
        m_shipObject = std::make_shared<VoxelObject>(m_shipPrototype);
        m_shipObject->body()->transform().setPosition({0.0f, 0.0f, 15.0f});

        m_stationObject = std::make_shared<VoxelObject>(m_stationPrototype);
        m_stationObject->body()->transform().setPosition({-16.0f, 40.0f, -200.0f});
        m_stationObject->body()->setAngularVelocity({0.0f, 0.0f, 0.05f});

        m_blockObject = std::make_shared<VoxelObject>(m_blockPrototype);
        m_blockObject->body()->transform().setPosition({30.0f, 20.0f, -100.0f});

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

        WeaponConfig weaponConfig;
        weaponConfig.cooldown = 1.0f / 2.0f;
        weaponConfig.meshID = m_bulletMeshID;

        m_weapon.reset(weaponConfig, *m_hailstormManager);
    }

    void onFrame(float seconds) override
    {
        m_playerInput->update(seconds);
        m_flightControl->update(seconds);

        m_physicsWorld.update(seconds);
        m_hailstormManager->update(seconds);

        glm::vec3 offset;
        offset.z = m_shipObject->prototype()->cluster().size().z * 1.4f;
        offset.y = m_shipObject->prototype()->cluster().size().y * 2;

        m_dolly->update(m_shipObject->pose().position() + m_shipObject->pose().orientation() * offset,
                        m_shipObject->pose().orientation(), seconds);

        {
            auto fireOrigin = m_shipObject->pose().position() + m_shipObject->pose().orientation() * glm::vec3(0, 0, -15);

            auto mouseNearPlane = (input().mousePosition() + 1.0f) / 2.0f;
            auto nearPlane = m_camera.nearPlane();

            auto mouseWorld = nearPlane.origin() + mouseNearPlane.x * nearPlane.right() + mouseNearPlane.y * nearPlane.up();
            auto fireDirection = mouseWorld - m_camera.position();

            if (input().mouseButtonDown(InputBase::MouseButton_Right))
            {
                m_weapon->setFireRequest(true,
                                         fireOrigin,
                                         fireDirection);
            }
            else
            {
                m_weapon->setFireRequest(false);
            }
            m_weapon->update(seconds);
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

    Optional<FlightControl>
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
    std::shared_ptr<VoxelObjectPrototype>
                m_shipPrototype;
    std::shared_ptr<VoxelObjectPrototype>
                m_stationPrototype;
    std::shared_ptr<VoxelObjectPrototype>
                m_blockPrototype;

    Optional<HailstormManager>
                m_hailstormManager;

    HailstormMeshID
                m_bulletMeshID;

    Optional<Weapon>
                m_weapon;
};

int main(int argc, char *argv[])
{
    return Game().run(argc, argv);
}
