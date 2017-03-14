#include "NpcControllerPrototype.h"

#include <Deliberation/ECS/Entity.h>

#include "NpcController.h"

NpcControllerPrototype::NpcControllerPrototype(const Json & json)
{

}

void NpcControllerPrototype::applyToEntity(Entity & entity) const
{
    entity.addComponent<NpcController>();
}