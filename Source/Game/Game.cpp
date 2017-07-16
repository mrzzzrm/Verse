#include <iostream>

#include <Deliberation/Core/Json.h>
#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/ECS/Level.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>
#include <Deliberation/Physics/Narrowphase.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/CameraDolly3D.h>
#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>
#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>
#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>
#include <Deliberation/Scene/Debug/DebugGeometryNode.h>
#include <Deliberation/Scene/Pipeline/RenderSystem.h>
#include <Deliberation/Scene/UVSphere.h>

#include <Deliberation/Deliberation.h>


#include <Voxel/VoxelRigidBodyPayload.h>
#include <Voxel/VoxelClusterPrimitiveTest.h>

#include "AimHelper.h"
#include "Emitter.h"
#include "Equipment.h"
#include "EquipmentPrototype.h"
#include "VersePrototypeManager.h"
#include "VersePrototypeSystem.h"
#include "CollisionShapeTypes.h"
#include "Hardpoint.h"
#include "VfxRenderer.h"
#include "VoxelClusterMarchingCubesTriangulation.h"
#include "VoxelClusterMarchingCubes.h"
#include "VoxelClusterContact.h"
#include "VoxelObject.h"
#include "VoxelWorld.h"
#include "VoxReader.h"
#include "Player/PlayerFlightControl.h"
#include "VerseApplication.h"
#include "Weapon.h"

using namespace deliberation;

class Game:
    public VerseApplication
{
public:
    Game():
        VerseApplication("Verse")
    {

    }

    void onApplicationStartup() override {
        auto & prototypeManager = m_world.systemRef<VersePrototypeSystem>().manager();

        m_level = std::make_shared<Level>(prototypeManager, GameDataPath("Data/Levels/level0.json"));
        m_level->reload();

        prototypeManager->updateEntities();

//        auto debugGroundPlaneRenderer = m_world.systemRef<RenderSystem>().renderManager().addRenderer<DebugGroundPlaneRenderer>();
//        debugGroundPlaneRenderer->setQuadSize(1.0f);
//        debugGroundPlaneRenderer->setSize(500.0f);
//        debugGroundPlaneRenderer->setRadius(250.0f);
//        debugGroundPlaneRenderer->setRenderToGBuffer(true);
//
//        auto debugNode = m_world.system<RenderSystem>()->debugGeometryRenderer()->addNode();
//        auto & pose = debugNode->addPose(Pose3D::atPosition({0.0f, 0.0f, 0.0f}));
//        pose.setTransform(Transform3D::withScale(10.0f));
    }

private:
    std::shared_ptr<Level> m_level;
};

int main(int argc, char *argv[])
{
    return Game().run(argc, argv);
}
