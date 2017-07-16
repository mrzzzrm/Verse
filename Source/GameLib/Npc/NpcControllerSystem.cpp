#include "NpcControllerSystem.h"

NpcControllerSystem::NpcControllerSystem(World & world)
    : Base(
          world,
          ComponentFilter::requires<
              NpcController,
              RigidBodyComponent,
              Equipment,
              FlightControlConfig>())
{
}

void NpcControllerSystem::onEntityAdded(Entity & entity)
{
    entity.addComponent<NpcFlightControl>();
}

void NpcControllerSystem::onEntityPrePhysicsUpdate(
    Entity & entity, float seconds)
{
    auto & body = *entity.component<RigidBodyComponent>().value();
    auto & equipment = entity.component<Equipment>();
    auto & npcController = entity.component<NpcController>();
    auto & flightControl = entity.component<NpcFlightControl>();
    auto & flightControlConfig = entity.component<FlightControlConfig>();

    auto & task = npcController.task();

    if (task) task->update(npcController, body, equipment, seconds);

    npcController.update(body, flightControl, flightControlConfig, seconds);
    flightControl.update(body, flightControlConfig, seconds);
}