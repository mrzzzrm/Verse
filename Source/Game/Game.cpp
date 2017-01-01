#include <iostream>

#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Platform/Application.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/CameraDolly3D.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>

#include <Deliberation/Voxel/VoxelClusterMarchingCubesTriangulation.h>
#include <Deliberation/Voxel/VoxelClusterMarchingCubes.h>
#include <Deliberation/Voxel/VoxReader.h>

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
        }

        m_camera.setPosition({0.0f, 1000.0f, 3.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)context().backbuffer().width() / context().backbuffer().height());

        m_clear = context().createClear();

        m_navigator.reset(m_camera, inputAdapter(), 10.0f);

        m_dolly.reset(m_camera);
    }

    void onFrame(float seconds) override
    {
        glm::vec3 offset;
        offset.z = - (m_cluster->size().z / 2 + 5);
        offset.y = 5;

        m_dolly->update(m_transform.position() + m_transform.orientation() * offset,
                        m_transform.orientation(), seconds);

        m_navigator->update(seconds);

        m_clear.schedule();

        m_draw.uniform("ViewProjection").set(m_camera.viewProjection());
        m_draw.uniform("Transform").set(m_transform.matrix());
        m_draw.schedule();
    }

private:
    Clear       m_clear;
    Optional<DebugGroundPlaneRenderer>
                m_ground;
    Camera3D    m_camera;
    Optional<DebugCameraNavigator3D>
                m_navigator;

    Optional<CameraDolly3D>
                m_dolly;

    Optional<VoxelCluster<glm::vec3>>
                m_cluster;

    Program     m_program;
    Draw        m_draw;

    Transform3D m_transform;

    VoxelClusterMarchingCubesTriangulation
                m_marchingCubesTriangulation;
};

int main(int argc, char *argv[])
{
    return Game().run(argc, argv);
}
