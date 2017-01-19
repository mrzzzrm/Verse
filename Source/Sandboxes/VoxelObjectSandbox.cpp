#include <iostream>

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Platform/Application.h>
#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>

#include "VoxelRenderChunkTree.h"
#include "VoxelWorld.h"

using namespace deliberation;

class VoxelObjectSandbox:
    public Application
{
public:
    VoxelObjectSandbox():
        Application("VoxelObjectSandbox")
    {

    }

    void onStartup() override
    {
        m_voxelWorld.reset(context(), m_physicsWorld, m_camera);

        m_camera.setPosition({0.0f, 0.0f, 3.0f});
        m_camera.setOrientation(glm::quat({-0.0f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)context().backbuffer().width() / context().backbuffer().height());

        m_renderTree.reset(*m_voxelWorld, glm::uvec3(6, 12, 20));

        std::vector<Voxel> voxels;
        for (size_t z = 0; z < m_renderTree->size().z; z++)
        {
            for (size_t y = 0; y < m_renderTree->size().y; y++)
            {
                for (size_t x = 0; x < m_renderTree->size().x; x++)
                {
                    Voxel voxel({x, y, z}, {1.0f, 0.0f, 0.0f});
                    voxel.visible = x == 0 || x == m_renderTree->size().x - 1 ||
                        y == 0 || y == m_renderTree->size().y - 1 ||
                        z == 0 || z == m_renderTree->size().z - 1;

                    voxels.emplace_back(voxel);
                }
            }
        }

        std::vector<glm::uvec3> rvoxels;
        for (size_t z = 0; z < m_renderTree->size().z - 2; z++)
        {
            for (size_t y = 0; y < m_renderTree->size().y - 2; y++)
            {
                for (size_t x = 0; x < m_renderTree->size().x - 2; x++)
                {
                    rvoxels.emplace_back(x, y, z);
                }
            }
        }

        m_renderTree->addVoxels(voxels);
        m_renderTree->removeVoxels(rvoxels);

        m_navigator.reset(m_camera, input(), 5.0f);

        m_clear = context().createClear();
    }

    void onFrame(float seconds) override
    {
        m_navigator->update(seconds);
        m_clear.schedule();
        m_renderTree->schedule(Pose3D());
    }

private:
    Camera3D    m_camera;
    Clear       m_clear;

    Optional<VoxelRenderChunkTree>
                m_renderTree;

    PhysicsWorld
                m_physicsWorld;

    Optional<VoxelWorld>
                m_voxelWorld;

    Optional<DebugCameraNavigator3D>
                m_navigator;
};

int main(int argc, char *argv[])
{
    return VoxelObjectSandbox().run(argc, argv);
}
