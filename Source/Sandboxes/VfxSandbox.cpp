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
#include "EmitterInstance.h"
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

        {
            auto lifetime = std::make_shared<EmitterRandomLifetime>(2.5f, 5.0f);
            auto placement = std::make_shared<EmitterGaussianSphericalPlacement>(3.0f, 1.0f);
            auto velocity = std::make_shared<EmitterFixedDirection>(40.0f, 89.0f);
            auto intensity = std::make_shared<EmitterNoisyIntensity>(50, 0.0f);
            auto rotation = std::make_shared<EmitterViewBillboardStrategy>();
            auto color = std::make_shared<EmitterColorOverLifetime>(glm::vec4{0.4f, 0.4f, 0.9f, 0.2f},
                                                                    glm::vec4{0.4f, 0.4f, 0.9f, 0.0f});
            auto size = std::make_shared<EmitterSizeOverLifetime>(16.0f, 15.0f);

            m_emitterSmoke = std::make_shared<Emitter>(
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
            auto lifetime = std::make_shared<EmitterRandomLifetime>(0.9f, 1.2f);
            auto placement = std::make_shared<EmitterFixedPlacement>();
            auto velocity = std::make_shared<EmitterConeStrategy>(0.1f, 280.0f, 289.0f);
            auto intensity = std::make_shared<EmitterNoisyIntensity>(7, 0.0f);
            auto rotation = std::make_shared<EmitterViewBillboardStrategy>();
            auto color = std::make_shared<EmitterColorOverLifetime>(glm::vec4{1.0f, 0.8f, 0.0f, 0.8f},
                                                                    glm::vec4{0.8f, 0.8f, 0.0f, 0.5f});
            auto size = std::make_shared<EmitterSizeOverLifetime>(2.5f, 2.0f);

            m_emitterBursts = std::make_shared<Emitter>(
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
            auto lifetime = std::make_shared<EmitterRandomLifetime>(0.9f, 1.2f);
            auto placement = std::make_shared<EmitterFixedPlacement>();
            auto velocity = std::make_shared<EmitterFixedDirection>(80.0f, 89.0f);
            auto intensity = std::make_shared<EmitterNoisyIntensity>(120, 0.0f);
            auto rotation = std::make_shared<EmitterViewBillboardStrategy>();
            auto color = std::make_shared<EmitterColorOverLifetime>(glm::vec4{0.4f, 0.4f, 0.9f, 0.6f},
                                                                    glm::vec4{0.8f, 0.8f, 0.0f, 0.5f});
            auto size = std::make_shared<EmitterSizeOverLifetime>(7.0f, 1.0f);

            m_emitterAfterburner = std::make_shared<Emitter>(
                *m_vfxManager,
                m_vfxManager->baseParticleMeshId(),
                velocity,
                rotation,
                placement,
                intensity,
                lifetime,
                color,
                size);

            m_emitterAfterburner->addChild(m_emitterSmoke);
            m_emitterAfterburner->addChild(m_emitterBursts);
        }

        for (int i = 0; i < 10; i++)
        {
            auto emitterInstance = std::make_shared<EmitterInstance>(m_emitterAfterburner);

            auto pose = Pose3D::atOrientation(glm::quat(glm::vec3{glm::half_pi<float>(), 0.0f, 0.0f}));
            pose.setPosition({i * 10.0f, 10.0f, 0.0f});

            emitterInstance->setBasePose(pose);
            emitterInstance->setTargetPose(pose);

            m_instances.emplace_back(emitterInstance);

            m_vfxManager->addEmitterInstance(emitterInstance);
        }
    }

    void onSandboxUpdate(float seconds) override
    {
        for (auto i = 0; i < m_instances.size(); i++)
        {
            auto & instance = m_instances[i];
            auto radius = 50.0f + i * 25;

            Pose3D pose;
            pose.setPosition({std::cos(m_angle) * radius, 10.0f, std::sin(m_angle) * radius});
            pose.setOrientation(glm::quat(glm::vec3{0.0f, -m_angle, 0.0f}));

            instance->setTargetPose(pose);
        }

        //m_angle += seconds;
    }

private:
    std::shared_ptr<Emitter>       m_emitterSmoke;
    std::shared_ptr<Emitter>       m_emitterAfterburner;
    std::shared_ptr<Emitter>       m_emitterBursts;

    std::vector<std::shared_ptr<EmitterInstance>> m_instances;

    float m_angle = 0.0f;
};

int main(int argc, char *argv[])
{
    return VfxSandbox().run(argc, argv);
}
