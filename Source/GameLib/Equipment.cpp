#include "Equipment.h"

#include "Hardpoint.h"
#include "VfxManager.h"

Equipment::Equipment(VfxManager & vfxManager):
    m_vfxManager(vfxManager)
{}

void Equipment::addHardpoint(std::shared_ptr<Hardpoint> hardpoint)
{
    m_hardpoints.emplace_back(hardpoint);
}

void Equipment::setFireRequest(bool active, const glm::vec3 & target)
{
    for (auto & hardpoint : m_hardpoints) hardpoint->setFireRequest(active, target);
}

void Equipment::setWeapon(size_t slot, std::shared_ptr<Weapon> weapon)
{
    Assert(slot < m_hardpoints.size(), "");
    m_hardpoints[slot]->setWeapon(weapon);
}

size_t Equipment::numEngineSlots() const
{
    return m_engineSlots.size();
}

void Equipment::addEngineSlot(std::shared_ptr<EngineSlot> engineSlot)
{
    m_engineSlots.emplace_back(engineSlot);
    m_enabledEngineSlots.emplace(engineSlot->voxel(), engineSlot);
}

void Equipment::setEngine(size_t slot, std::shared_ptr<Engine> engine)
{
    Assert(slot < m_engineSlots.size(), "");

    auto & previousEngine = m_engineSlots[slot]->engine();

    if (previousEngine)
    {
        m_vfxManager.removeEmitterInstance(previousEngine->emitterInstance());
    }

    m_engineSlots[slot]->setEngine(engine);

    if (engine)
    {
        m_vfxManager.addEmitterInstance(engine->emitterInstance());
    }
}

void Equipment::update(float seconds, const EquipmentUpdateContext & context)
{
    for (auto & hardpoint : m_hardpoints) hardpoint->update(seconds, context);
    for (auto & engineSlot : m_engineSlots) engineSlot->setTargetPose(context.targetPose);
}

void Equipment::receive(const VoxelObjectModification & modification)
{

    for (const auto & voxel : modification.additions)
    {

    }

    for (const auto & voxel : modification.removals)
    {
        auto it = m_enabledEngineSlots.find(voxel);
        if (it == m_enabledEngineSlots.end()) continue;

        if (it->second->engine()) m_vfxManager.removeEmitterInstance(it->second->engine()->emitterInstance());

        m_disabledEngineSlots.emplace(*it);
        m_enabledEngineSlots.erase(it);
    }
}