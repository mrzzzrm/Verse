#include <iostream>
#include <thread>

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Platform/Application.h>
#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>

#include "Emitter.h"
#include "HailstormManager.h"
#include "VoxelRenderChunkTree.h"
#include "VoxelWorld.h"
#include "VoxelClusterPrimitiveTest.h"

using namespace deliberation;

class VfxSandbox:
    public Application
{
public:
    VfxSandbox():
        Application("VfxSandbox")
    {

    }

    void onStartup() override
    {
        m_voxelWorld.reset(context(), m_physicsWorld, m_camera);

        m_camera.setPosition({0.0f, 400.0f, 500.0f});
        m_camera.setOrientation(glm::quat({-1.0f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)context().backbuffer().width() / context().backbuffer().height());

        m_navigator.reset(m_camera, input(), 150.0f);

        m_clear = context().createClear();

        m_hailstormManager.reset(context(), m_camera, m_physicsWorld, *m_voxelWorld);

        auto particleMesh = UVSphere(5, 5).generateMesh2();
        auto particleMeshID = m_hailstormManager->renderer().addMesh(particleMesh);

        {
            auto lifetime = std::make_shared<EmitterRandomLifetime>(1.0f, 1.5f);
            auto placement = std::make_shared<EmitterGaussianSphericalPlacement>(5.0f, 2.0f);
            auto velocity = std::make_shared<EmitterConeStrategy>(glm::pi<float>() / 8.0f, 10.0f, 150.0f);
            auto intensity = std::make_shared<EmitterNoisyIntensity>(100, 0.4f);

            m_emitter0.reset(*m_hailstormManager, particleMeshID, velocity, placement, intensity, lifetime);
        }
        {
            auto lifetime = std::make_shared<EmitterRandomLifetime>(0.5f, 0.7f);
            auto placement = std::make_shared<EmitterGaussianSphericalPlacement>(2.0f, 1.0f);
            auto velocity = std::make_shared<EmitterConeStrategy>(glm::pi<float>() / 25.0f, 150.0f, 700.0f);
            auto intensity = std::make_shared<EmitterNoisyIntensity>(200, 0.1f);

            m_emitter1.reset(*m_hailstormManager, particleMeshID, velocity, placement, intensity, lifetime);
        }

        m_groundPlane.reset(context(), m_camera);
        m_groundPlane->setSize(1000.0f);
        m_groundPlane->setQuadSize(100.0f);
        m_groundPlane->setRadius(750.0f);
    }

    void onFrame(float seconds) override
    {
        m_clear.schedule();
        m_groundPlane->schedule();

        m_angle -= seconds;
        auto radius = 140.0f;
        auto x = std::cos(m_angle) * radius;
        auto y = std::sin(m_angle) * radius;

        auto newPose = m_emitter0->pose();

        newPose.setPosition(glm::vec3(x, 20, -y));
        newPose.worldRotate(glm::quat({0.0f, -seconds, 0.0f}));

        m_emitter0->update(seconds, newPose);
        m_emitter1->update(seconds, newPose);

        m_physicsWorld.update(seconds);
        m_hailstormManager->update(seconds);
        m_navigator->update(seconds);
        m_voxelWorld->update(seconds);

       // std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

private:
    Camera3D                m_camera;
    Clear                   m_clear;
    PhysicsWorld            m_physicsWorld;
    Optional<DebugCameraNavigator3D>
                            m_navigator;
    Optional<HailstormManager>
                            m_hailstormManager;
    Optional<VoxelWorld>    m_voxelWorld;
    Optional<DebugGroundPlaneRenderer>
                            m_groundPlane;

    Optional<Emitter>       m_emitter0;
    Optional<Emitter>       m_emitter1;
    float                   m_angle = 0.0f;
};

int main(int argc, char *argv[])
{
    return VfxSandbox().run(argc, argv);
}
