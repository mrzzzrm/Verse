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
#include "ResourceManager.h"
#include "VfxSystem.h"
#include "VfxManager.h"
#include "VoxelRenderable.h"
#include "VoxelWorld.h"
#include "VerseApplication.h"

using namespace deliberation;

class VfxSandbox:
    public VerseApplication
{
public:
    VfxSandbox():
        VerseApplication("VfxSandbox", VerseApplicationSystemInitMode::NoSystems)
    {

    }

    void onApplicationStartup() override
    {
        EnableGLErrorChecksAndLogging();

        m_camera.setPosition({0.0f, 10.0f, 0.0f});
        m_camera.setOrientation(glm::quat({-glm::half_pi<float>(), 0.0f, 0.0f}));

        m_world.addSystem<ResourceManager>();
        m_world.addSystem<VfxSystem>(m_camera);

        m_navigator = std::make_shared<DebugCameraNavigator3D>(m_camera, input(), 1000.0f);
//        m_ground = std::make_shared<DebugGroundPlaneRenderer>(drawContext(), m_camera);
//        m_ground->setQuadSize(10.0f);
//        m_ground->setRadius(80.0f);
//        m_ground->setSize(10.0f);

        {
            auto lifetime = std::make_shared<EmitterRandomLifetime>(5.5f, 8.0f);
            auto placement = std::make_shared<EmitterGaussianSphericalPlacement>(1.0f, 1.0f);
            auto velocity = std::make_shared<EmitterAnyDirection>(0.2f, 0.5f);
            auto intensity = std::make_shared<EmitterBurstIntensity>(30, 0.0f);
            auto rotation = std::make_shared<EmitterViewBillboardStrategy>();
            auto color = std::make_shared<EmitterColorOverLifetime>(glm::vec4{0.4f, 0.4f, 0.4f, 0.4f},
                                                                    glm::vec4{0.4f, 0.4f, 0.4f, 0.0f});
            auto size = std::make_shared<EmitterSizeOverLifetime>(3.0f, 7.0f);

            m_emitterSmoke = std::make_shared<Emitter>(
                m_world.systemRef<VfxSystem>().manager(),
                m_world.systemRef<VfxSystem>().manager().getOrCreateMeshId(R::ParticleMesh),
                velocity,
                rotation,
                placement,
                intensity,
                lifetime,
                color,
                size);
        }
//        {
//            auto lifetime = std::make_shared<EmitterRandomLifetime>(5.5f, 8.0f);
//            auto placement = std::make_shared<EmitterGaussianSphericalPlacement>(1.0f, 1.0f);
//            auto velocity = std::make_shared<EmitterAnyDirection>(0.2f, 0.5f);
//            auto intensity = std::make_shared<EmitterNoisyIntensity>(30, 0.0f);
//            auto rotation = std::make_shared<EmitterViewBillboardStrategy>();
//            auto color = std::make_shared<EmitterColorOverLifetime>(glm::vec4{0.4f, 0.4f, 0.4f, 0.4f},
//                                                                    glm::vec4{0.4f, 0.4f, 0.4f, 0.0f});
//            auto size = std::make_shared<EmitterSizeOverLifetime>(3.0f, 7.0f);
//
//            m_emitterSmoky = std::make_shared<Emitter>(
//                *m_vfxManager,
//                m_vfxManager->getOrCreateMeshId(R::ParticleMesh),
//                velocity,
//                rotation,
//                placement,
//                intensity,
//                lifetime,
//                color,
//                size);
//        }

        for (int i = 0; i < 10; i++)
        {
            auto emitterInstance = std::make_shared<EmitterInstance>(m_emitterSmoke);

            auto pose = Pose3D::atOrientation(glm::quat(glm::vec3{glm::half_pi<float>(), 0.0f, 0.0f}));
            pose.setPosition({i * 10.0f, 10.0f, 0.0f});

            emitterInstance->setBasePose(pose);
            emitterInstance->setTargetPose(pose);

            m_instances.emplace_back(emitterInstance);

            m_world.systemRef<VfxSystem>().manager().addEmitterInstance(emitterInstance);
        }
//        for (int i = 0; i < 10; i++)
//        {
//            auto emitterInstance = std::make_shared<EmitterInstance>(m_emitterSmoky);
//
//            auto pose = Pose3D::atOrientation(glm::quat(glm::vec3{glm::half_pi<float>(), 0.0f, 0.0f}));
//            pose.setPosition({i * 10.0f, 10.0f, -20.0f});
//
//            emitterInstance->setBasePose(pose);
//            emitterInstance->setTargetPose(pose);
//
//            m_instances.emplace_back(emitterInstance);
//
//            m_vfxManager->addEmitterInstance(emitterInstance);
//        }

//        m_debugGeometryRenderer.emplace(*m_debugGeometryManager);
    }

    void onApplicationUpdate(float seconds) override
    {
        m_navigator->update(seconds);
    }

    void onApplicationRender() override
    {
//        m_ground->render();
    }

private:
    std::shared_ptr<Emitter>                        m_emitterSmoke;
    std::shared_ptr<Emitter>                        m_emitterSmoky;

    std::vector<std::shared_ptr<EmitterInstance>>   m_instances;

    std::shared_ptr<DebugCameraNavigator3D>         m_navigator;
    std::shared_ptr<DebugGroundPlaneRenderer>       m_ground;
};

int main(int argc, char *argv[])
{
    return VfxSandbox().run(argc, argv);
}
