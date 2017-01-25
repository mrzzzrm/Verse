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

        m_camera.setPosition({0.0f, 0.0f, 10.0f});
        m_camera.setOrientation(glm::quat({-0.0f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)context().backbuffer().width() / context().backbuffer().height());

        m_voxelData.reset(*m_voxelWorld, glm::uvec3(10,10,10));

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

//        std::vector<glm::uvec3> rvoxels;
//        for (size_t z = 0; z < m_voxelData->size().z; z++)
//        {
//            for (size_t y = 0; y < m_voxelData->size().y; y++)
//            {
//                for (size_t x = 0; x < m_voxelData->size().x; x++)
//                {
//                    auto a = m_voxelData->size() - 1u;
//                    auto b = a - glm::uvec3(1,0,0);

//                    if (glm::uvec3(x, y, z) == a) continue;
//                    if (glm::uvec3(x, y, z) == b) continue;

//                    //rvoxels.emplace_back(x, y, z);
//                    m_voxelData->removeVoxels({glm::uvec3(x, y, z)});
//                }
//            }
//        }


        m_object.reset(*m_voxelData);

        m_navigator.reset(m_camera, input(), 5.0f);

        m_clear = context().createClear();
    }

    void onFrame(float seconds) override
    {
//        if (!m_removalDone) {
//            m_removalCooldown -= seconds;
//            if (m_removalCooldown < 0) {
//                m_object->removeVoxels({m_removalPointer});
//
//                m_removalPointer.x++;
//                if (m_removalPointer.x == m_object->data().size().x) {
//                    m_removalPointer.x = 0;
//                    m_removalPointer.y++;
//                    if (m_removalPointer.y == m_object->data().size().y) {
//                        m_removalPointer.y = 0;
//                        m_removalPointer.z++;
//                        if (m_removalPointer.z == m_object->data().size().z) {
//                            m_removalDone = true;
//                        }
//                    }
//                }
//
//                m_removalCooldown = 0.5f;
//            }
//        }

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
            auto hit = m_object->data().shapeTree().lineCast(Transform3D(), Ray3D(origin, fireDirection), voxel);

            if (hit)
            {
                auto r = 5;
                for (i32 z = (i32)voxel.z - r; z <= (i32)voxel.z + r; z++)
                for (i32 y = (i32)voxel.y - r; y <= (i32)voxel.y + r; y++)
                for (i32 x = (i32)voxel.x - r; x <= (i32)voxel.x + r; x++)
                {
                    auto v = glm::vec3(x, y, z);
                    if (glm::length(v - glm::vec3(voxel)) > r) continue;

                    if (m_object->data().cluster().contains(v) && m_object->data().cluster().test(v))
                    {
                        m_object->removeVoxels({v});
                    }
                }
            }
        }

        m_clear.schedule();
        m_object->schedule();
//
//        if (!m_removalDone)
//        {
//          //  m_object->removeVoxels({glm::uvec3(0,0,0)});
//            m_removalDone = true;
//        }
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

    float m_removalCooldown = 0.2f;
    glm::uvec3 m_removalPointer;
    bool m_removalDone = false;
};

int main(int argc, char *argv[])
{
    return VoxelObjectSandbox().run(argc, argv);
}
