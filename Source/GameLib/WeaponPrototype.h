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

    size_t vfxMeshRenderBatchIndex() const { return m_vfxMeshRenderBatchIndex; }
    void setVfxMeshRenderBatchIndex(size_t vfxBatchIndex) const { m_vfxMeshRenderBatchIndex = vfxBatchIndex; }

    VfxMeshId vfxMeshId() const { return m_vfxMeshId; }
    float lifetime() const { return m_lifetime; }
    float speed() const { return m_speed; }
    float frequency() const { return m_frequency; }
    float damage() const { return m_damage; }
    float scale() const { return m_scale; }

protected:
    void onReload(const Json & json) override;

private:
    VfxMeshId   m_vfxMeshId = INVALID_VFX_MESH_ID;
    float       m_lifetime = 0.0f;
    float       m_speed = 0.0f;
    float       m_frequency = 0.0f;
    float       m_damage = 0.0f;
    float       m_scale = 0.0f;

    // The first weapon encountering this unset will need to set it. Reset whenever the Prototype changes
    mutable     size_t m_vfxMeshRenderBatchIndex = INVALID_VFX_MESH_RENDER_BATCH_INDEX;
};