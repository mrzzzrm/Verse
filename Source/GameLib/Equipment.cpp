#include "Equipment.h"

#include "Hardpoint.h"

const std::vector<std::shared_ptr<Hardpoint>> & Equipment::hardpoints() const
{
    return m_hardpoints;
}

void Equipment::addHardpoint(std::shared_ptr<Hardpoint> hardpoint)
{
    m_hardpoints.emplace_back(hardpoint);
}

void Equipment::setFireRequest(bool active, const glm::vec3 & target)
{
    for (auto & hardpoint : m_hardpoints) hardpoint->setFireRequest(active, target);
}

void Equipment::update(float seconds, const Pose3D & referencePose)
{
    for (auto & hardpoint : m_hardpoints) hardpoint->update(seconds, referencePose);
}