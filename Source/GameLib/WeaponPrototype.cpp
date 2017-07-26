#include "WeaponPrototype.h"

#include <memory>

#include <Deliberation/ECS/World.h>

#include <Deliberation/Resource/PrototypeManager.h>

#include "HailstormManager.h"
#include "ResourceManager.h"

WeaponPrototype::WeaponPrototype(const std::weak_ptr<PrototypeManager> &prototypeManager):
    AbstractPrototype(prototypeManager)
{}

void WeaponPrototype::reload(const Json & json)
{
    auto & world = prototypeManagerRef().world();
    auto & resourceManager = world.systemRef<ResourceManager>();
    auto & hailstormManager = world.systemRef<HailstormManager>();

    auto resourceToken = resourceManager.resourceToken<std::shared_ptr<MeshData>>(json["Bullet"]);
    m_vfxMeshId = hailstormManager.vfxManager().getOrCreateMeshId(resourceToken);

    m_lifetime = json["Lifetime"];
    m_speed = json["Speed"];
    m_frequency = json["Frequency"];
    m_damage = json["Damage"];

    m_vfxRenderBatchIndex = INVALID_VFX_RENDER_BATCH_INDEX;
}
