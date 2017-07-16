#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/ECS/Component.h>
#include <Deliberation/ECS/Entity.h>

#include "DockingPoint.h"
#include "EngineSlot.h"
#include "GameLib.h"
#include "Hardpoint.h"
#include "VoxelLight.h"
#include "VoxelObjectModification.h"

namespace deliberation
{
class Transform3D;
class Pose3D;
} // namespace deliberation

class VfxManager;
class Weapon;

struct EquipmentUpdateContext
{
    Pose3D    targetPose;
    glm::vec3 angularVelocity;
    glm::vec3 linearVelocity;
    Entity    entity;
};

class Equipment final : public Component<Equipment>
{
    DELIBERATION_COMPONENT_NAME("Equipment")

public:
    const std::shared_ptr<VfxManager> & vfxManager() const
    {
        return m_vfxManager;
    }
    void setVfxManager(const std::shared_ptr<VfxManager> & vfxManager)
    {
        m_vfxManager = vfxManager;
    }

    const std::vector<std::shared_ptr<Hardpoint>> &    hardpoints() const;
    const std::vector<std::shared_ptr<EngineSlot>> &   engineSlots() const;
    const std::vector<std::shared_ptr<DockingPoint>> & dockingPoints() const
    {
        return m_dockingPoints;
    }
    const std::vector<std::shared_ptr<Attachment>> & attachments() const
    {
        return m_attachments;
    }

    /**
     * @return Bullet speed to be used for predictive aiming
     */
    float bulletSpeed() const;

    void clearFireRequests();
    void setFireRequestDirectionForAllHardpoints(const glm::vec3 & direction);
    void setFireRequestTargetForAllHardpoints(
        const Transform3D & equipmentTransform,
        const glm::vec3 &   equipmentVelocity,
        const glm::vec3 &   targetPosition,
        const glm::vec3 &   targetVelocity);

    void setWeapon(size_t slot, std::shared_ptr<Weapon> weapon);
    void setEngine(size_t slot, std::shared_ptr<Engine> engine);

    void addAttachment(const std::shared_ptr<Attachment> & attachment);

    void update(float seconds, const EquipmentUpdateContext & context);

private:
    friend class EquipmentPrototype;
    friend class EquipmentSystem;

private:
    std::shared_ptr<VfxManager>                m_vfxManager;
    std::vector<std::shared_ptr<Hardpoint>>    m_hardpoints;
    std::vector<std::shared_ptr<EngineSlot>>   m_engineSlots;
    std::vector<std::shared_ptr<DockingPoint>> m_dockingPoints;

    std::vector<std::shared_ptr<Attachment>> m_attachments;
    std::unordered_map<glm::uvec3, std::shared_ptr<Attachment>>
        m_attachmentByVoxel;
};