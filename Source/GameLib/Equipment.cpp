#include "Equipment.h"

#include "Hardpoint.h"
#include "VfxManager.h"

Equipment::Equipment(VfxManager & vfxManager):
    m_vfxManager(vfxManager)
{}

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

size_t Equipment::numEngineSlots() const
{
    return m_engineSlots.size();
}

void Equipment::addEngineSlot(std::shared_ptr<EngineSlot> engineSlot)
{
    m_engineSlots.emplace_back(engineSlot);
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

    for (auto & engineSlot : m_engineSlots)
    {
        engineSlot->setTargetPose(context.targetPose);
    }
}