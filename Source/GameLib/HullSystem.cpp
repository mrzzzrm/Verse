#include "HullSystem.h"

#include "HullComponent.h"
#include "VoxelObjectBulletHit.h"
#include "VoxelObject.h"
#include "VoxelShredder.h"

HullSystem::HullSystem(World & world)
    : Base(world, ComponentFilter::requires<VoxelObject, HullComponent>())
{
}

void HullSystem::onCreated() { subscribeEvent<VoxelObjectBulletHit>(); }

void HullSystem::onEvent(const VoxelObjectBulletHit & hit)
{
    if (!hit.entity.hasComponent<HullComponent>()) return;
    if (!hit.entity.hasComponent<VoxelObject>()) return;

    auto &     hull = hit.entity.component<HullComponent>();
    const auto prevHealth = hull.health();

    Log->info("Applying Hull damage: {} - {}", hull.health(), hit.damage);

    hull.setHealth(std::max(0.0f, hull.health() - hit.damage));

    if (prevHealth != 0.0f && hull.health() == 0)
    {
        Entity entity(hit.entity);
        VoxelShredder().explode(entity);
    }
}