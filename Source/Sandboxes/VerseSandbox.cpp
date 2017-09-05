#include <iostream>

#include <btBulletDynamicsCommon.h>

#include <Deliberation/Deliberation.h>
#include <Deliberation/Physics/CollisionShape.h>
#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>
#include <Deliberation/Physics/SphereShape.h>
#include <Voxel/VoxelShape.h>
#include <VoxReader.h>

using namespace deliberation;

#include <iostream>

#include <btBulletDynamicsCommon.h>

int main()
{
    deliberation::init();


    PhysicsWorld physicsWorld;
    physicsWorld.setGravity({0.0f, 0.0f, 0.0f});


    auto models = VoxReader().read(GameDataPath("Data/VoxelClusters/Factory2.vox"));
    auto & model = models[0];
    auto shape = std::make_shared<VoxelShape>(model.size);
    for (const auto & voxel : model.voxels)
    {
        shape->updateVoxel(voxel.cell, true);
    }

    Log->info("Mass Props: {} {} {}", shape->mass(1.0f), shape->centerOfMass(), shape->localInertia(1.0f));

    auto body = std::make_shared<RigidBody>(shape);

    body->applyImpulse({4.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f});
    ;

    Log->info("Angular: {}; Local: {}", body->angularVelocity(),body->localVelocity({1.0f, 0.0f, 0.0f}));

    return 0;
}
