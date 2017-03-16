#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/ECS/Component.h>

#include "Hardpoint.h"
#include "EngineSlot.h"
#include "GameLib.h"
#include "VoxelObjectModification.h"

namespace deliberation
{
class Pose3D;
}

class VfxManager;
class Weapon;

struct EquipmentUpdateContext
{
    Pose3D      targetPose;
    glm::vec3   angularVelocity;
    glm::vec3   linearVelocity;
};

struct EquipmentDesc
{
    std::vector<HardpointDesc> hardpointDescs;
    std::vector<EngineSlotDesc> engineSlotDescs;
};

class Equipment final:
    public Component<Equipment, ComponentSubscriptions<Equipment, VoxelObjectModification>>
{
public:
    Equipment(VfxManager & vfxManager, const EquipmentDesc & desc);

    size_t numHardpoints() const;
    void setFireRequest(bool active, const glm::vec3 & target);
    void setWeapon(size_t slot, std::shared_ptr<Weapon> weapon);

    size_t numEngineSlots() const;
    void setEngine(size_t slot, std::shared_ptr<Engine> engine);

    void update(float seconds, const EquipmentUpdateContext & context);

    void receive(const VoxelObjectModification & modification);

private:
    VfxManager &                                m_vfxManager;
    std::vector<std::shared_ptr<Hardpoint>>     m_hardpoints;
    std::vector<std::shared_ptr<EngineSlot>>    m_engineSlots;

    std::unordered_map<glm::uvec3, std::shared_ptr<ItemSlot>>
                                                m_itemSlotByVoxel;
};