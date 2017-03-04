#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/ECS/Component.h>

#include "EngineSlot.h"
#include "GameLib.h"
#include "VoxelObjectModification.h"

namespace deliberation
{
class Pose3D;
}

class Hardpoint;
class VfxManager;

struct EquipmentUpdateContext
{
    Pose3D      targetPose;
    glm::vec3   angularVelocity;
    glm::vec3   linearVelocity;
};

class Equipment final:
    public Component<Equipment, ComponentSubscriptions<Equipment, VoxelObjectModification>>
{
public:
    Equipment(VfxManager & vfxManager);

    const std::vector<std::shared_ptr<Hardpoint>> & hardpoints() const;
    void addHardpoint(std::shared_ptr<Hardpoint> hardpoint);
    void setFireRequest(bool active, const glm::vec3 & target);

    size_t numEngineSlots() const;
    void addEngineSlot(std::shared_ptr<EngineSlot> engineSlot);
    void setEngine(size_t slot, std::shared_ptr<Engine> engine);

    void update(float seconds, const EquipmentUpdateContext & context);

    void receive(const VoxelObjectModification & modification);

private:
    VfxManager &                                m_vfxManager;
    std::vector<std::shared_ptr<Hardpoint>>     m_hardpoints;
    std::vector<std::shared_ptr<EngineSlot>>    m_engineSlots;

    std::unordered_map<glm::uvec3, std::shared_ptr<EngineSlot>>
                                                m_enabledEngineSlots;
    std::unordered_map<glm::uvec3, std::shared_ptr<EngineSlot>>
                                                m_disabledEngineSlots;
};