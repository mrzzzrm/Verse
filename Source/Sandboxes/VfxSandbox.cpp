#include <iostream>

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Platform/Application.h>
#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/UVSphere.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>

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


        m_camera.setPosition({0.0f, 0.0f, 10.0f});
        m_camera.setOrientation(glm::quat({-0.0f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)context().backbuffer().width() / context().backbuffer().height());

        m_navigator.reset(m_camera, input(), 55.0f);

        m_clear = context().createClear();

        m_hailstormManager.reset(context(), m_camera, m_physicsWorld, *m_voxelWorld);

        auto particleMesh = UVSphere(5, 5).generateMesh2();
        auto particleMeshID = m_hailstormManager->renderer().addMesh(particleMesh);

        {
            auto lifetime = std::make_shared<EmitterRandomLifetime>(1.0f, 1.5f);
            auto velocity = std::make_shared<EmitterConeStrategy>(glm::pi<float>() / 8.0f, 10.0f, 100.0f);
            auto intensity = std::make_shared<EmitterNoisyIntensity>(100, 0.4f);

            m_emitter0.reset(*m_hailstormManager, particleMeshID, velocity, intensity, lifetime);
        }
        {
            auto lifetime = std::make_shared<EmitterRandomLifetime>(1.0f, 1.5f);
            auto velocity = std::make_shared<EmitterConeStrategy>(glm::pi<float>() / 15.0f, 50.0f, 300.0f);
            auto intensity = std::make_shared<EmitterNoisyIntensity>(200, 0.4f);

            m_emitter1.reset(*m_hailstormManager, particleMeshID, velocity, intensity, lifetime);
        }
    }

    void onFrame(float seconds) override
    {
        m_clear.schedule();

        m_emitter0->pose().worldRotate({0.0f, seconds, 0.0f});
        m_emitter1->pose().worldRotate({0.0f, seconds, 0.0f});

        m_emitter0->update(seconds, m_emitter0->pose());
        m_emitter1->update(seconds, m_emitter1->pose());
        m_physicsWorld.update(seconds);
        m_hailstormManager->update(seconds);
        m_navigator->update(seconds);
        m_voxelWorld->update(seconds);
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
    Optional<Emitter>       m_emitter0;
    Optional<Emitter>       m_emitter1;
};

int main(int argc, char *argv[])
{
    return VfxSandbox().run(argc, argv);
}
