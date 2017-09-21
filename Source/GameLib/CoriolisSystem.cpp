#include "CoriolisSystem.h"

#include <Deliberation/ECS/RigidBodyComponent.h>
#include <Deliberation/Physics/RigidBody.h>

#include "CoriolisComponent.h"

CoriolisSystem::CoriolisSystem(World & world)
    : Base(
          world,
          ComponentFilter::requires<CoriolisComponent, RigidBodyComponent>())
{
}

void CoriolisSystem::onEntityGameUpdate(Entity & entity, const UpdateFrame & updateFrame)
{
    auto & coriolis = entity.component<CoriolisComponent>();
    auto & body = entity.component<RigidBodyComponent>().value();

    body->setAngularVelocity(coriolis.angularVelocity());
}