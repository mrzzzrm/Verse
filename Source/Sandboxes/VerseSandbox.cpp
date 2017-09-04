#include <iostream>

#include <btBulletDynamicsCommon.h>

#include <Deliberation/Deliberation.h>
#include <Deliberation/Physics/CollisionShape.h>
#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>
#include <Deliberation/Physics/SphereShape.h>
#include <Voxel/VoxelShape.h>

using namespace deliberation;

#include <iostream>

#include <btBulletDynamicsCommon.h>

int main (void)
{
    deliberation::init();

    PhysicsWorld physicsWorld;
    physicsWorld.setGravity({0.0f, 0.0f, 0.0f});

    auto shape = std::make_shared<VoxelShape>(glm::uvec3{2, 2, 2});
    shape->updateVoxel({0,0,0}, true);

    Transform3D transform;
    transform.setPosition({1.0f, 5.0f, -4.0f});
    transform.setOrientation(glm::quat(glm::vec3(1.0f, 0.5f, -1.4f)));

    auto rigidBody = std::make_shared<RigidBody>(shape, transform);
    rigidBody->setLinearVelocity({10.0f, 0.0f, 0.0f});
    physicsWorld.addRigidBody(rigidBody);

    for (int i = 0; i < 1000; i++) {
        physicsWorld.update(1.3f / 60.f);
        std::cout << i
                  << ";"
                  <<rigidBody->transform().position().x
                  <<";"
                  <<rigidBody->bulletRigidBody()->getWorldTransform().getOrigin().x() << std::endl;
    }

//    btQuaternion bQuat;
//    glm::quat gQuat;
//
//    bQuat.setRotation(btVector3(2.1f, -1.6f, 1.0f).normalized(), 1.0f);
//    bQuat.normalize();
//    Log->info("Bullet Z: {} {}", bQuat, quatRotate(bQuat, btVector3(1.9f, -2.0f, 1.0f)));
//
//    gQuat = glm::angleAxis(1.0f, glm::normalize(glm::vec3{2.1f, -1.6f, 1.0f}));
//    Log->info("GLM Z: {} {}", gQuat, gQuat * glm::vec3(1.9f, -2.0f, 1.0f));
//
//    auto b2g = BulletPhysicsConvert(bQuat);
//    auto g2b = BulletPhysicsConvert(gQuat);
//
//    Log->info("{}", b2g);
//    Log->info("{}", g2b);

    return 0;
}
