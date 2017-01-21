#include <iostream>

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Platform/Application.h>
#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>

#include "VoxelRenderChunkTree.h"
#include "VoxelWorld.h"
#include "VoxelClusterPrimitiveTest.h"

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

        m_voxelData.reset(*m_voxelWorld, glm::uvec3(20, 20, 20));

        std::vector<Voxel> voxels;
        for (size_t z = 0; z < m_voxelData->size().z; z++)
        {
            for (size_t y = 0; y < m_voxelData->size().y; y++)
            {
                for (size_t x = 0; x < m_voxelData->size().x; x++)
                {
                    Voxel voxel({x, y, z}, {1.0f, 0.0f, 0.0f});
                    voxel.hull = x == 0 || x == m_voxelData->size().x - 1 ||
                        y == 0 || y == m_voxelData->size().y - 1 ||
                        z == 0 || z == m_voxelData->size().z - 1;

                    voxels.emplace_back(voxel);
                }
            }
        }
//
//        std::vector<glm::uvec3> rvoxels;
//        for (size_t z = 0; z < m_voxelData->size().z - 5; z++)
//        {
//            for (size_t y = 0; y < m_voxelData->size().y - 6; y++)
//            {
//                for (size_t x = 0; x < m_voxelData->size().x - 5; x++)
//                {
//                    rvoxels.emplace_back(x, y, z);
//                }
//            }
//        }

        m_voxelData->addVoxels(voxels);
//        m_voxelData->removeVoxels(rvoxels);

        m_object.reset(*m_voxelData);

        m_navigator.reset(m_camera, input(), 5.0f);

        m_clear = context().createClear();
    }

    void onFrame(float seconds) override
    {
        m_navigator->update(seconds);

        if (input().keyDown(InputBase::Key_SPACE))
        {
            auto mouseNearPlane = (input().mousePosition() + 1.0f) / 2.0f;
            auto nearPlane = m_camera.nearPlane();
            auto mouseWorld = nearPlane.origin() + mouseNearPlane.x * nearPlane.right() + mouseNearPlane.y * nearPlane.up();
            auto fireDirection = (mouseWorld - m_camera.position()) * 1000.0f;
            auto origin = m_camera.position();
            auto direction = fireDirection;

            glm::uvec3 voxel;
            auto hit = m_voxelData->shapeTree().lineCast(Transform3D(), Ray3D(origin, fireDirection), voxel);

            if (hit) m_object->removeVoxels({voxel});
        }

        m_clear.schedule();
        m_object->schedule();
    }

private:
    Camera3D    m_camera;
    Clear       m_clear;

    Optional<VoxelObjectVoxelData>
                m_voxelData;

    Optional<VoxelObject>
                m_object;

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
