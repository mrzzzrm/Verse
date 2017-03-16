#pragma once

#include <memory>

#include <Deliberation/Core/Math/Pose3D.h>

#include "GameLib.h"
#include "ItemSlot.h"

class EquipmentUpdateContext;
class Weapon;

struct HardpointDesc:
    ItemSlotDesc
{
    float maxAngle = 0.0f;
};

class Hardpoint final:
    public ItemSlot
{
public:
    Hardpoint(const HardpointDesc & desc);

    const std::shared_ptr<Weapon> & weapon() const;

    void setFireRequest(bool active, const glm::vec3 & direction);

    /**
     * Only used for warping the Hardpoint, otherwise use update()'s referencePose
     */
    void setReferencePose(const Pose3D & referencePose);

    void setWeapon(std::shared_ptr<Weapon> weapon);

    void update(float seconds, const EquipmentUpdateContext & context);

protected:
    void onDisabled() override;

private:
    float                   m_maxAngle = glm::half_pi<float>();
    Pose3D                  m_referencePose;
    std::shared_ptr<Weapon> m_weapon;
};