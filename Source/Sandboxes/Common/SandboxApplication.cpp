#include "SandboxApplication.h"

#include "VoxelClusterSplitSystem.h"

#include "VfxSystem.h"

SandboxApplication::SandboxApplication(const std::string & name):
    Application(name)
{

}

void SandboxApplication::onStartup()
{
    m_physicsWorld.narrowphase().registerPrimitiveTest((int)::CollisionShapeType::VoxelCluster,
                                                       std::make_unique<VoxelClusterPrimitiveTest>());

    m_physicsWorld.narrowphase().contactDispatcher().
        registerContactType<VoxelClusterContact>((int)::CollisionShapeType::VoxelCluster);

    m_voxelWorld.reset(context(), m_physicsWorld, m_camera);

    m_camera.setPosition({0.0f, 200.0f, 300.0f});
    m_camera.setOrientation(glm::quat({-1.0f, 0.0f, 0.0f}));
    m_camera.setAspectRatio((float)context().backbuffer().width() / context().backbuffer().height());

    m_navigator.reset(m_camera, input(), 150.0f);

    m_clear = context().createClear();

    m_groundPlane.reset(context(), m_camera);
    m_groundPlane->setSize(1000.0f);
    m_groundPlane->setQuadSize(100.0f);
    m_groundPlane->setRadius(750.0f);

    m_vfxManager.emplace(context(), m_camera, *m_voxelWorld);

    m_world.addSystem<PhysicsWorldSystem>(m_physicsWorld);
    m_world.addSystem<VoxelObjectSystem>(m_physicsWorld, *m_voxelWorld);
    m_world.addSystem<NpcControllerSystem>();
    m_hailstormManager = m_world.addSystem<HailstormManager>(context(), m_camera, m_physicsWorld, *m_voxelWorld);
    m_world.addSystem<VoxelClusterSplitSystem>();
    m_world.addSystem<VfxSystem>(*m_vfxManager);
    //     m_world.addSystem<NpcDebugRendererSystem>(context(), m_camera);


    m_debugGeometryManager.emplace(context());

    onSandboxStartup();
}

void SandboxApplication::onFrame(float seconds)
{
    auto physicsSimulationSeconds = m_physicsWorld.nextSimulationStep(seconds);

    if (EpsilonGt(physicsSimulationSeconds, 0.0f))
    {
        m_world.prePhysicsUpdate(physicsSimulationSeconds);
        m_physicsWorld.update(seconds);
        m_world.update(seconds);
        m_hailstormManager->update(physicsSimulationSeconds);
        m_vfxManager->update(physicsSimulationSeconds);

        onSandboxPhysicsUpdate(physicsSimulationSeconds);
    }
    else
    {
        m_physicsWorld.update(seconds);
    }

    onSandboxUpdate(seconds);

    m_navigator->update(seconds);

    m_clear.render();
    m_world.render();
    m_groundPlane->render();
    m_voxelWorld->render();
    m_hailstormManager->render();
    m_vfxManager->render();

    onSandboxRender();
}
