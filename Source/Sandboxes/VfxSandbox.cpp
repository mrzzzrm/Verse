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
#include "VfxManager.h"
#include "VoxelRenderChunkTree.h"
#include "VoxelWorld.h"
#include "VoxelClusterPrimitiveTest.h"

#include "SandboxApplication.h"

using namespace deliberation;

class VfxSandbox:
    public SandboxApplication
{
public:
    VfxSandbox():
        SandboxApplication("VfxSandbox")
    {

    }

    void onSandboxStartup() override
    {
        auto particleMesh = UVSphere(5, 5).generateMesh2();
        auto particleMeshID = m_vfxManager->renderer().addMesh(particleMesh);
//
//        {
//            auto lifetime = std::make_shared<EmitterRandomLifetime>(1.0f, 1.5f);
//            auto placement = std::make_shared<EmitterGaussianSphericalPlacement>(5.0f, 2.0f);
//            auto velocity = std::make_shared<EmitterConeStrategy>(glm::pi<float>() / 8.0f, 10.0f, 150.0f);
//            auto intensity = std::make_shared<EmitterNoisyIntensity>(100, 0.4f);
//
//            m_emitter0.reset(*m_hailstormManager, particleMeshID, velocity, placement, intensity, lifetime);
//        }
//        {
//            auto lifetime = std::make_shared<EmitterRandomLifetime>(0.5f, 0.7f);
//            auto placement = std::make_shared<EmitterGaussianSphericalPlacement>(2.0f, 1.0f);
//            auto velocity = std::make_shared<EmitterConeStrategy>(glm::pi<float>() / 25.0f, 150.0f, 700.0f);
//            auto intensity = std::make_shared<EmitterNoisyIntensity>(200, 0.1f);
//
//            m_emitter1.reset(*m_hailstormManager, particleMeshID, velocity, placement, intensity, lifetime);
//        }

        {
            auto lifetime = std::make_shared<EmitterRandomLifetime>(1.0f, 1.5f);
            auto placement = std::make_shared<EmitterFixedPlacement>();
            auto velocity = std::make_shared<EmitterConeStrategy>(glm::pi<float>() / 8.0f, 10.0f, 150.0f);
            auto intensity = std::make_shared<EmitterNoisyIntensity>(100, 0.4f);

            m_emitterCenter.reset(*m_vfxManager, particleMeshID, velocity, placement, intensity, lifetime);
        }
    }

    void onSandboxUpdate(float seconds) override
    {
        m_emitterCenter->update(seconds, Pose3D::atOrientation(glm::quat(glm::vec3{glm::half_pi<float>(), 0.0f, 0.0f})));
//        m_angle -= seconds;
//        auto radius = 140.0f;
//        auto x = std::cos(m_angle) * radius;
//        auto y = std::sin(m_angle) * radius;
//
//        auto newPose = m_emitter0->pose();
//
//        newPose.setPosition(glm::vec3(x, 20, -y));
//        newPose.worldRotate(glm::quat({0.0f, -seconds, 0.0f}));
//
//        m_emitter0->update(seconds, newPose);
//        m_emitter1->update(seconds, newPose);
    }

private:
    Optional<Emitter>       m_emitterCenter;
    Optional<Emitter>       m_emitter0;
    Optional<Emitter>       m_emitter1;
    float                   m_angle = 0.0f;
};

int main(int argc, char *argv[])
{
    return VfxSandbox().run(argc, argv);
}
