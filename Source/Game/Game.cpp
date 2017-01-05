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

#include "Hailstorm/HailstormRenderer.h"

#include "Voxel/VoxelClusterMarchingCubesTriangulation.h"
#include "Voxel/VoxelClusterMarchingCubes.h"
#include "Voxel/VoxelClusterShape.h"
#include "Voxel/VoxelClusterContact.h"
#include "Voxel/VoxReader.h"

#include "PlayerInput.h"
#include "FlightControl.h"

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
        VoxReader voxReader;

        auto clusters = voxReader.read(deliberation::dataPath("Data/VoxelCluster/ship.vox"));
        if (!clusters.empty())
        {
            auto marchingCubes = VoxelClusterMarchingCubes(m_marchingCubesTriangulation, clusters[0]);
            marchingCubes.run();

            m_program = context().createProgram({deliberation::dataPath("Data/Shaders/VoxelExample.vert"),
                                                 deliberation::dataPath("Data/Shaders/VoxelExample.frag")});
            m_draw = context().createDraw(m_program);

            m_cluster.reset(clusters[0]);

            m_draw.addVertices(marchingCubes.takeVertices());

            m_transform.setCenter(glm::vec3(m_cluster->size()) / 2.0f);
            m_transform.setPosition({16.0f, 20.0f, 0.0f});
        }

        {
            auto stationClusters = voxReader.read(deliberation::dataPath("Data/VoxelCluster/station.vox"));
            if (!stationClusters.empty())
            {
                auto marchingCubes = VoxelClusterMarchingCubes(m_marchingCubesTriangulation, stationClusters[0], 12.0f);
                marchingCubes.run();

                m_stationDraw = context().createDraw(m_program);

                m_stationCluster.reset(stationClusters[0]);

                m_stationDraw.addVertices(marchingCubes.takeVertices());

                m_stationTransform.setCenter((glm::vec3(m_stationCluster->size()) / 2.0f) * 12.0f);
                m_stationTransform.setPosition({-16.0f, 40.0f, -100.0f});
                m_stationTransform.setOrientation(glm::quat({glm::pi<float>() * 0.1f, glm::pi<float>() * 0.1f, 0.0f}));
            }

        }

        m_camera.setPosition({0.0f, 10.0f, 3.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)context().backbuffer().width() / context().backbuffer().height());

        m_clear = context().createClear();

//        m_navigator.reset(m_camera, input(), 20.0f);

        m_dolly.reset(m_camera);

        m_debugGeometryManager.reset(context());
        m_debugGeometryRenderer.reset(*m_debugGeometryManager);

//        m_debugGeometryRenderer->addPose({});
//        m_debugGeometryRenderer->addPose({});

        m_debugGroundPlaneRenderer.reset(context(), m_camera);
        m_debugGroundPlaneRenderer->setQuadSize(5.0f);
        m_debugGroundPlaneRenderer->setSize(100.0f);
        m_debugGroundPlaneRenderer->setRadius(50.0f);

        m_shipShape = std::make_shared<VoxelClusterShape>(*m_cluster);
        m_shipBody = std::make_shared<RigidBody>(m_shipShape, m_transform);

        m_physicsWorld.narrowphase().contactDispatcher().
            registerContactType<VoxelClusterContact>((int)::CollisionShapeType::VoxelCluster);

        m_physicsWorld.addRigidBody(m_shipBody);

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

        m_flightControl.reset(m_shipBody, playerShipConfig);

        m_playerInput.reset(input(), m_camera, *m_flightControl);


        m_hailstormRenderer.reset(context(), m_camera);

        auto bulletMesh = UVSphere(5, 5).generateMesh2();
        m_bulletMeshID = m_hailstormRenderer->addMesh(bulletMesh);
    }

    void onFrame(float seconds) override
    {
        m_playerInput->update(seconds);
        m_flightControl->update(seconds);

        m_physicsWorld.update(seconds);

        glm::vec3 offset;
        offset.z = m_cluster->size().z * 1.4f;
        offset.y = m_cluster->size().y * 2;

        m_dolly->update(m_transform.position() + m_transform.orientation() * offset,
                        m_transform.orientation(), seconds);

//        m_navigator->update(seconds);

//        Pose3D cameraPose;
//        cameraPose.setPosition(m_testCamera.position());
//        cameraPose.setOrientation(m_testCamera.orientation());
//        m_debugGeometryRenderer->pose(0).setPose(cameraPose);

        m_clear.schedule();

        m_debugGeometryRenderer->schedule(m_camera);

        m_transform = m_shipBody->transform();

        m_draw.uniform("ViewProjection").set(m_camera.viewProjection());
        m_draw.uniform("Transform").set(m_transform.matrix());
        m_draw.schedule();

        m_stationTransform.localRotate(glm::quat({0.0f, 0.0f, seconds * 0.05f}));
        m_stationDraw.uniform("ViewProjection").set(m_camera.viewProjection());
        m_stationDraw.uniform("Transform").set(m_stationTransform.matrix());
        m_stationDraw.schedule();

        m_hailstormRenderer->update(seconds);

        //m_debugGroundPlaneRenderer->schedule();
    }

private:
    Clear       m_clear;
    Optional<DebugGroundPlaneRenderer>
                m_ground;
    Camera3D    m_camera;
    Camera3D    m_testCamera;
    Optional<DebugCameraNavigator3D>
                m_navigator;

    Optional<CameraDolly3D>
                m_dolly;

    Optional<VoxelCluster<glm::vec3>>
                m_cluster;

    Program     m_program;
    Draw        m_draw;


    Optional<VoxelCluster<glm::vec3>>
                m_stationCluster;
    Draw        m_stationDraw;
    Transform3D m_stationTransform;

    Transform3D m_transform;

    VoxelClusterMarchingCubesTriangulation
                m_marchingCubesTriangulation;

    Optional<DebugGeometryManager>
                m_debugGeometryManager;
    Optional<DebugGeometryRenderer>
                m_debugGeometryRenderer;

    Optional<DebugGroundPlaneRenderer>
                m_debugGroundPlaneRenderer;

    PhysicsWorld
                m_physicsWorld;
    std::shared_ptr<VoxelClusterShape>
                m_shipShape;
    std::shared_ptr<RigidBody>
                m_shipBody;

    Optional<FlightControl>
                m_flightControl;

    Optional<PlayerInput>
                m_playerInput;


    Optional<HailstormRenderer>
                m_hailstormRenderer;

    HailstormRenderer::MeshID
                m_bulletMeshID;
};

int main(int argc, char *argv[])
{
    return Game().run(argc, argv);
}
