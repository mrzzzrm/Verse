#include "WeaponPrototype.h"

#include <memory>

#include <Deliberation/ECS/World.h>

#include <Deliberation/Resource/PrototypeManager.h>

#include "HailstormManager.h"
#include "Deliberation/Resource/ResourceManager.h"

WeaponPrototype::WeaponPrototype(const std::weak_ptr<PrototypeManager> &prototypeManager):
    AbstractPrototype(prototypeManager)
{}

void WeaponPrototype::onReload(const Json & json)
{
    auto & hailstormManager = GetGlobal<World>()->systemRef<HailstormManager>();

    auto resourceToken = GetGlobal<ResourceManager>()->resourceToken<std::shared_ptr<MeshData>>(json["Bullet"]);
    m_vfxMeshId = hailstormManager.vfxManager().getOrCreateMeshId(resourceToken);

    m_vfxMeshRenderBatchIndex = INVALID_VFX_MESH_RENDER_BATCH_INDEX;

    m_lifetime = json["Lifetime"];
    m_speed = json["Speed"];
    m_frequency = json["Frequency"];
    m_damage = json["Damage"];
    m_scale = json["Scale"];

    // Explosion Emitter
    {
        auto iter = json.find("Explosion");
        if (iter != json.end())
        {
            m_explosionEmitter = prototypeManagerRef().getOrCreatePrototype<Emitter>(*iter);
        }
    }

}
