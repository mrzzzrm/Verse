#pragma once

#include <Deliberation/Core/Json.h>
#include <Deliberation/Resource/AbstractPrototype.h>
#include <Deliberation/Resource/ResourceToken.h>

#include "GameLib.h"
#include "VfxDefines.h"

namespace deliberation
{
class World;
class Transform3D;
}

class WeaponPrototype:
    public AbstractPrototype
{
public:
    explicit WeaponPrototype(const std::weak_ptr<PrototypeManager> &prototypeManager);

    size_t vfxRenderBatchIndex() const { return m_vfxRenderBatchIndex; }
    void setVfxRenderBatchIndex(size_t vfxBatchIndex) const { m_vfxRenderBatchIndex = vfxBatchIndex; }

    VfxMeshId vfxMeshId() const { return m_vfxMeshId; }
    float lifetime() const { return m_lifetime; }
    float speed() const { return m_speed; }
    float frequency() const { return m_frequency; }
    float damage() const { return m_damage; }

    void reload(const Json & json);

private:
    VfxMeshId   m_vfxMeshId = INVALID_VFX_MESH_ID;
    float       m_lifetime = 0.0f;
    float       m_speed = 0.0f;
    float       m_frequency = 0.0f;
    float       m_damage = 0.0f;

    // The first weapon encountering this unset will need to set it. Reset whenever the Prototype changes
    mutable     size_t m_vfxRenderBatchIndex = INVALID_VFX_MESH_RENDER_BATCH_INDEX;
};