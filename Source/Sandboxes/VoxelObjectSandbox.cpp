#include <iostream>

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Platform/Application.h>
#include <Deliberation/Scene/Camera3D.h>

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

        m_renderTree.reset(glm::uvec3(3, 3, 1));

        std::vector<Voxel> voxels;
        for (size_t z = 0; z < m_renderTree->size().z; z++)
        {
            for (size_t y = 0; y < m_renderTree->size().y; y++)
            {
                for (size_t x = 0; x < m_renderTree->size().x; x++)
                {
                    Voxel voxel({x, y, z}, {1.0f, 0.0f, 0.0f});
                    voxels.emplace_back(voxel);
                }
            }
        }

        m_renderTree->addVoxels(voxels);
    }

    void onFrame(float seconds) override
    {
        m_renderTree->schedule(*m_voxelWorld);
    }

private:
    Camera3D    m_camera;

    Optional<VoxelRenderChunkTree>
                m_renderTree;

    PhysicsWorld
                m_physicsWorld;

    Optional<VoxelWorld>
                m_voxelWorld;
};

int main(int argc, char *argv[])
{
    return VoxelObjectSandbox().run(argc, argv);
}
