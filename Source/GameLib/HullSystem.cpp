#include "HullSystem.h"

#include "HullComponent.h"
#include "VoxelObjectBulletHit.h"

HullSystem::HullSystem(World & world):
    Base(world, ComponentFilter::requires<VoxelObject, HullComponent>())
{

}

void HullSystem::onCreated()
{
    subscribeEvent<VoxelObjectBulletHit>();
}

void HullSystem::onEvent(const VoxelObjectBulletHit & hit)
{
    auto & hull = hit.entity.component<HullComponent>();
    const auto prevHealth = hull.health();

    hull.setHealth(std::max(0.0f, hull.health() - hit.damage));

    if (prevHealth != 0.0f && hull.health() == 0)
    {
        hit.entity.scheduleRemoval();
    }
}