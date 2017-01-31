#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "GameLib.h"

namespace deliberation
{
class Pose3D;
}

class Hardpoint;

class Equipment final
{
public:
    const std::vector<std::shared_ptr<Hardpoint>> & hardpoints() const;
    void addHardpoint(std::shared_ptr<Hardpoint> hardpoint);
    void setFireRequest(bool active, const glm::vec3 & target);

    void update(float seconds, const Pose3D & referencePose);

private:
    std::vector<std::shared_ptr<Hardpoint>> m_hardpoints;
};