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
//            auto lifetime = std::make_shared<EmitterRandomLifetime>(0.2f, 1.0f);
//            auto placement = std::make_shared<EmitterFixedPlacement>();
//            auto velocity = std::make_shared<EmitterAnyDirection>(300.0f, 400.0f);
//            auto intensity = std::make_shared<EmitterBurstIntensity>(100, 0.5f);
//            auto rotation = std::make_shared<EmitterViewBillboardStrategy>();
//            auto color = std::make_shared<EmitterConstColor>(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
//            auto size = std::make_shared<EmitterSizeOverLifetime>(3.0f, 0.0f);
//
//            m_emitterCenter.reset(
//                *m_vfxManager,
//                m_vfxManager->baseParticleMeshId(),
//                velocity,
//                rotation,
//                placement,
//                intensity,
//                lifetime,
//                color,
//                size);
        }


        {
            auto lifetime = std::make_shared<EmitterRandomLifetime>(3.0f, 5.0f);
            auto placement = std::make_shared<EmitterGaussianSphericalPlacement>(10.0f, 0.3f);
            auto velocity = std::make_shared<EmitterAnyDirection>(0.8f, 4.3f);
            auto intensity = std::make_shared<EmitterNoisyIntensity>(100, 0.5f);
            auto rotation = std::make_shared<EmitterViewBillboardStrategy>();
            auto color = std::make_shared<EmitterColorOverLifetime>(glm::vec4{0.9f, 0.4f, 0.0f, 0.5f},
                                                                    glm::vec4{0.8f, 0.8f, 0.8f, 1.0f});
            auto size = std::make_shared<EmitterSizeOverLifetime>(10.0f, 0.0f);

            m_emitterSmoke.reset(
                *m_vfxManager,
                m_vfxManager->baseParticleMeshId(),
                velocity,
                rotation,
                placement,
                intensity,
                lifetime,
                color,
                size);
        }

        {
            auto lifetime = std::make_shared<EmitterRandomLifetime>(0.2f, 0.4f);
            auto placement = std::make_shared<EmitterGaussianCircularPlacement>(1.0f, 0.3f);
            auto velocity = std::make_shared<EmitterConeStrategy>(0.02f, 250.0f, 350.0f);
            auto intensity = std::make_shared<EmitterNoisyIntensity>(100, 0.0f);
            auto rotation = std::make_shared<EmitterViewBillboardStrategy>();
            auto color = std::make_shared<EmitterColorOverLifetime>(glm::vec4{0.2f, 0.2f, 0.9f, 1.0f},
                                                                    glm::vec4{0.8f, 0.8f, 0.8f, 0.0f});
            auto size = std::make_shared<EmitterSizeOverLifetime>(5.0f, 15.0f);

            m_emitterAfterburner.reset(
                *m_vfxManager,
                m_vfxManager->baseParticleMeshId(),
                velocity,
                rotation,
                placement,
                intensity,
                lifetime,
                color,
                size);
        }
    }

    void onSandboxUpdate(float seconds) override
    {
        auto pose = Pose3D::atOrientation(glm::quat(glm::vec3{glm::half_pi<float>(), 0.0f, 0.0f}));
        pose.setPosition({0.0f, 10.0f, 0.0f});
//        m_emitterCenter->setPose(pose);
//        m_emitterCenter->update(seconds, pose);

//        m_emitterSmoke->setPose(pose);
//        m_emitterSmoke->update(seconds, pose);

        m_emitterAfterburner->setPose(pose);
        m_emitterAfterburner->update(seconds, pose);


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
    Optional<Emitter>       m_emitterSmoke;
    Optional<Emitter>       m_emitterAfterburner;
    float                   m_angle = 0.0f;
};

int main(int argc, char *argv[])
{
    return VfxSandbox().run(argc, argv);
}
