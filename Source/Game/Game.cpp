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
#include "CollisionShapeTypes.h"
#include "HailstormRenderer.h"
#include "VoxelClusterMarchingCubesTriangulation.h"
#include "VoxelClusterMarchingCubes.h"
#include "VoxelClusterContact.h"
#include "VoxelObject.h"
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
            auto models = voxReader.read(deliberation::dataPath("Data/VoxelCluster/ship.vox"));
            if (!models.empty())
            {
                m_shipData = std::make_shared<VoxelObjectVoxelData>(*m_voxelWorld, models[0].size);
                m_shipData->addVoxels(models[0].voxels);
            }
        }
        {
            auto models = voxReader.read(deliberation::dataPath("Data/VoxelCluster/station.vox"));
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

        m_engineEmitter.reset(*m_hailstormManager, particleMeshIDs);

        auto bulletMesh = UVSphere(5, 5).generateMesh2();
        m_bulletMeshID = m_hailstormManager->renderer().addMesh(bulletMesh);

        m_engineEmitter.reset(*m_hailstormManager, {m_bulletMeshID, m_bulletMeshID, m_bulletMeshID});

        WeaponConfig weaponConfig;
        weaponConfig.cooldown = 1.0f / 2.0f;
        weaponConfig.meshID = m_bulletMeshID;

        m_weapon.reset(weaponConfig, *m_hailstormManager, m_shipObject->id().worldUID);
    }

    void onFrame(float seconds) override
    {
        m_engineEmitter->update(seconds);

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

            auto fireOrigin = m_shipObject->pose().position() + m_shipObject->pose().orientation() * glm::vec3(0, 0, -15);
            auto fireDirection = target - fireOrigin;

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

            if (input().keyPressed(InputBase::Key_SPACE))
            {
                auto origin = m_camera.position();
                auto direction = fireDirection * 1000.0f;

                m_physicsWorld.lineCast(Ray3D(m_camera.position(), direction),
                                        [&](const RayCastIntersection &intersection) -> bool {
                                            if (intersection.body.shape()->type() ==
                                                (int) ::CollisionShapeType::VoxelCluster) {
                                                auto &voxelClusterIntersection =
                                                    static_cast<const RayCastVoxelClusterIntersection &>(intersection);

                                                if (voxelClusterIntersection.object.lock()->id().worldUID ==
                                                    m_shipObject->id().worldUID) {
                                                    std::cout << "Skipped because of UID" << std::endl;
                                                    return true;
                                                }

                                                std::cout << "Hit voxel: " << voxelClusterIntersection.voxel << " of "
                                                          << voxelClusterIntersection.object.lock()->id().worldUID
                                                          << std::endl;

                                                voxelClusterIntersection.object.lock()->removeVoxels({
                                                                          voxelClusterIntersection.voxel});
                                            } else {
                                                std::cout << "Skipped because not a voxelCluster" << std::endl;
                                            }

                                            return false;
                                        });
            }
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

    Optional<Weapon>
                m_weapon;
};

int main(int argc, char *argv[])
{
    return Game().run(argc, argv);
}
