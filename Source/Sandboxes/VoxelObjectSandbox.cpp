#include <iostream>

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Platform/App.h>
#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>

#include "VoxelClusterPrimitiveTest.h"
#include "VoxelRenderable.h"
#include "VoxelWorld.h"

using namespace deliberation;

class VoxelObjectSandbox : public App
{
public:
    VoxelObjectSandbox() : App("VoxelObjectSandbox") {}

    void onStartup() override
    {
        m_voxelWorld.reset(m_physicsWorld, m_camera);

        m_camera.setPosition({0.0f, 0.0f, 10.0f});
        m_camera.setOrientation(glm::quat({-0.0f, 0.0f, 0.0f}));
        m_camera.setAspectRatio(
            (float)GetGlobal<DrawContext>()->backbuffer().width() /
            drawContext().backbuffer().height());

        m_voxelData.reset(*m_voxelWorld, glm::uvec3(2, 1, 1));

        std::vector<Voxel> voxels;
        for (size_t z = 0; z < m_voxelData->size().z; z++)
        {
            for (size_t y = 0; y < m_voxelData->size().y; y++)
            {
                for (size_t x = 0; x < m_voxelData->size().x; x++)
                {
                    Voxel voxel({x, y, z}, {1.0f, 0.0f, 0.0f});
                    voxels.emplace_back(voxel);
                }
            }
        }

        m_voxelData->addVoxels(voxels);

        m_object0.reset(*m_voxelData);
        m_object0->setPose(Pose3D::atPosition({-3.0f, 0.0f, 0.0f}));

        m_object1.reset(*m_voxelData);
        m_object1->setPose(Pose3D::atPosition({3.0f, 0.0f, 0.0f}));

        m_navigator.reset(m_camera, input(), 25.0f);

        m_clear = drawContext().createClear();

        m_object0->removeVoxels({glm::uvec3(0, 0, 0)});
    }

    void onFrame(const UpdateFrame & updateFrame) override
    {
        m_clear.render();

        m_navigator->update(updateFrame);

        m_clear.render();
        m_object0->schedule();
        m_object1->schedule();
    }

private:
    Camera3D m_camera;
    Clear    m_clear;

    Optional<VoxelObjectVoxelData> m_voxelData;

    Optional<VoxelObject> m_object0;
    Optional<VoxelObject> m_object1;

    PhysicsWorld m_physicsWorld;

    Optional<VoxelWorld> m_voxelWorld;

    Optional<DebugCameraNavigator3D> m_navigator;
};

int main(int argc, char * argv[])
{
    return VoxelObjectSandbox().run(argc, argv);
}
