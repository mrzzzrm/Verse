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
};