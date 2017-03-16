#include "Equipment.h"

#include "Hardpoint.h"
#include "VfxManager.h"

Equipment::Equipment(VfxManager & vfxManager, const EquipmentDesc & desc):
    m_vfxManager(vfxManager)
{
    for (const auto & hardpointDesc : desc.hardpointDescs)
    {
        auto hardpoint = std::make_shared<Hardpoint>(hardpointDesc);
        m_hardpoints.emplace_back(hardpoint);
        m_itemSlotByVoxel.emplace(hardpoint->voxel(), hardpoint);
    }

    for (const auto & engineSlotDesc : desc.engineSlotDescs)
    {
        auto engineSlot = std::make_shared<EngineSlot>(engineSlotDesc);
        engineSlot->setVfxManager(m_vfxManager);
        m_engineSlots.emplace_back(engineSlot);
        m_itemSlotByVoxel.emplace(engineSlot->voxel(), engineSlot);
    }
}

const std::vector<std::shared_ptr<Hardpoint>> & Equipment::hardpoints() const
{
    return m_hardpoints;
}

void Equipment::setFireRequest(bool active, const glm::vec3 & direction)
{
    for (auto & hardpoint : m_hardpoints) hardpoint->setFireRequest(active, direction);
}

void Equipment::setWeapon(size_t slot, std::shared_ptr<Weapon> weapon)
{
    Assert(slot < m_hardpoints.size(), "");
    m_hardpoints[slot]->setWeapon(weapon);
}

const std::vector<std::shared_ptr<EngineSlot>> & Equipment::engineSlots() const
{
    return m_engineSlots;
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
    for (const auto & voxel : modification.removals)
    {
        auto it = m_itemSlotByVoxel.find(voxel);
        if (it == m_itemSlotByVoxel.end()) continue;

        it->second->setEnabled(false);
    }
}