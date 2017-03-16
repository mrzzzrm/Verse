#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Deliberation/Core/Json.h>
#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/ECS/Entity.h>

#include "ComponentPrototype.h"
#include "Equipment.h"
#include "GameLib.h"
#include "ItemSlot.h"

class VfxManager;

class EquipmentPrototype final:
    public ComponentPrototype
{
public:
    EquipmentPrototype(const Json & json, VfxManager & vfxManager);

    void applyToEntity(Entity & entity) const override;

private:
    void loadSlotDesc(const Json & obj, ItemSlotDesc & slot) const;

private:
    VfxManager &    m_vfxManager;
    EquipmentDesc   m_desc;
};