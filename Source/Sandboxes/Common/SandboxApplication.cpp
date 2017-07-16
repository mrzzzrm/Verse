//#include "SandboxApplication.h"
//
//#include <Deliberation/Draw/TextureLoader.h>
//#include <Deliberation/ECS/Systems/ApplicationSystem.h>
//
//#include "VoxelClusterSplitSystem.h"
//
//#include "VfxSystem.h"
//
// SandboxApplication::SandboxApplication(const std::string & name):
//    Application(name)
//{
//
//}
//
// void SandboxApplication::onStartup()
//{
//    m_physicsWorld.narrowphase().registerPrimitiveTest((int)::CollisionShapeType::VoxelCluster,
//                                                       std::make_unique<VoxelClusterPrimitiveTest>());
//
//    m_physicsWorld.narrowphase().contactDispatcher().
//        registerContactType<VoxelClusterContact>((int)::CollisionShapeType::VoxelCluster);
//
//    m_camera.setPosition({0.0f, 200.0f, 300.0f});
//    m_camera.setOrientation(glm::quat({-1.0f, 0.0f, 0.0f}));
//    m_camera.setAspectRatio((float)drawContext().backbuffer().width() /
//    drawContext().backbuffer().height());
//
//    m_navigator.reset(m_camera, input(), 150.0f);
//
//    auto skyboxPaths = std::array<std::string, 6> {
//        deliberation::DeliberationDataPath("Data/Skybox/Cloudy/Right.png"),
//        deliberation::DeliberationDataPath("Data/Skybox/Cloudy/Left.png"),
//        deliberation::DeliberationDataPath("Data/Skybox/Cloudy/Top.png"),
//        deliberation::DeliberationDataPath("Data/Skybox/Cloudy/Bottom.png"),
//        deliberation::DeliberationDataPath("Data/Skybox/Cloudy/Back.png"),
//        deliberation::DeliberationDataPath("Data/Skybox/Cloudy/Front.png")
//    };
//
//    auto faceTexture = drawContext().createTexture(
//        TextureLoader(deliberation::DeliberationDataPath("Data/Skybox/Debug/Right.png")).load());
//
//    auto skyboxCubemapBinary = TextureLoader(skyboxPaths).load();
//    auto skyboxCubemap = drawContext().createTexture(skyboxCubemapBinary);
//
//    m_skyboxRenderer = std::make_shared<SkyboxRenderer>(drawContext(),
//    m_camera, skyboxCubemap);
//
////    m_voxelWorld.reset(drawContext(), m_physicsWorld, m_camera,
/// skyboxCubemap);
//
//    m_clear = drawContext().createClear();
//
//    m_groundPlane.reset(drawContext(), m_camera);
//    m_groundPlane->setSize(1000.0f);
//    m_groundPlane->setQuadSize(100.0f);
//    m_groundPlane->setRadius(750.0f);
//
//    m_vfxManager.emplace(drawContext(), m_camera, *m_voxelWorld);
//
//    m_world.addSystem<ApplicationSystem>(*this);
//    m_world.addSystem<PhysicsWorldSystem>(m_physicsWorld);
//    //m_world.addSystem<VoxelWorld>(m_camera, );
//    m_world.addSystem<NpcControllerSystem>();
//    m_hailstormManager = m_world.addSystem<HailstormManager>(drawContext(),
//    m_camera, m_physicsWorld, *m_voxelWorld);
//    m_world.addSystem<VoxelClusterSplitSystem>();
//    m_world.addSystem<VfxSystem>(*m_vfxManager);
//    //m_world.addSystem<NpcDebugRendererSystem>(drawContext(), m_camera);
//
//    m_debugGeometryManager.emplace(drawContext());
//
//
//    onSandboxStartup();
//}
//
// void SandboxApplication::onFrame(float seconds)
//{
//    auto physicsSimulationSeconds =
//    m_physicsWorld.nextSimulationStepSeconds(seconds);
//
//    if (EpsilonGt(physicsSimulationSeconds, 0.0f))
//    {
//        m_world.prePhysicsUpdate(physicsSimulationSeconds);
//        m_physicsWorld.update(seconds);
//        m_world.update(seconds);
//        m_hailstormManager->update(physicsSimulationSeconds);
//        m_vfxManager->update(physicsSimulationSeconds);
//
//        onSandboxPhysicsUpdate(physicsSimulationSeconds);
//    }
//    else
//    {
//        m_physicsWorld.update(seconds);
//    }
//
//    onSandboxUpdate(seconds);
//
//    m_navigator->update(seconds);
//
//    m_clear.render();
//    m_world.render();
//    m_groundPlane->render();
//    m_voxelWorld->render();
//    m_skyboxRenderer->render();
//
//    onSandboxRender();
//
//    m_hailstormManager->render();
//    m_vfxManager->render();
//}
