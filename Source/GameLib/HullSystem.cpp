#include "HullSystem.h"

#include "HullComponent.h"
#include "VoxelObjectBulletHit.h"
#include "VoxelObject.h"

HullSystem::HullSystem(World & world)
    : Base(world, ComponentFilter::requires<VoxelObject, HullComponent>())
{
}

void HullSystem::onCreated() { subscribeEvent<VoxelObjectBulletHit>(); }

void HullSystem::onEvent(const VoxelObjectBulletHit & hit)
{
    if (!hit.entity.hasComponent<HullComponent>()) return;

    auto &     hull = hit.entity.component<HullComponent>();
    const auto prevHealth = hull.health();

    hull.setHealth(std::max(0.0f, hull.health() - hit.damage));

    if (prevHealth != 0.0f && hull.health() == 0)
    {
        auto entity = hit.entity;
        entity.scheduleRemoval();
    }
}