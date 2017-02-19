#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include "GameLib.h"

namespace deliberation
{
class Pose3D;
}

class Hardpoint;

struct EquipmentUpdateContext
{
    Pose3D      targetPose;
    glm::vec3   angularVelocity;
    glm::vec3   linearVelocity;
};

class Equipment final
{
public:
    const std::vector<std::shared_ptr<Hardpoint>> & hardpoints() const;
    void addHardpoint(std::shared_ptr<Hardpoint> hardpoint);
    void setFireRequest(bool active, const glm::vec3 & target);

    void update(float seconds, const EquipmentUpdateContext & context);

private:
    std::vector<std::shared_ptr<Hardpoint>> m_hardpoints;
};