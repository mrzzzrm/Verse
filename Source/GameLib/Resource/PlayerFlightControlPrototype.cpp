#include "PlayerFlightControlPrototype.h"

#include <Deliberation/ECS/Entity.h>

#include "PlayerFlightControl.h"

PlayerFlightControlPrototype::PlayerFlightControlPrototype(const Json & json)
{

}

void PlayerFlightControlPrototype::applyToEntity(Entity & entity) const
{
    entity.addComponent<PlayerFlightControl>();
}