#include "EquipmentPrototype.h"

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Json.h>
#include <Deliberation/Core/StreamUtils.h>

#include "Equipment.h"
#include "Hardpoint.h"

EquipmentPrototype::EquipmentPrototype(nlohmann::json & json)
{
    for (const auto & obj : json["Engines"])
    {
        EnginePrototype prototype;
        loadSlotPrototype(obj, prototype);

        m_engines.emplace_back(prototype);
    }

    for (const auto & obj : json["Hardpoints"])
    {
        HardpointPrototype prototype;
        loadSlotPrototype(obj, prototype);

        prototype.maxAngle = obj["MaxAngle"];

        m_hardpoints.emplace_back(prototype);
    }
}

const std::vector<EnginePrototype> & EquipmentPrototype::engines() const
{
    return m_engines;
}

const std::vector<HardpointPrototype> & EquipmentPrototype::hardpoints() const
{
    return m_hardpoints;
}

void EquipmentPrototype::applyToEntity(Entity & entity, VfxManager & vfxManager) const
{
    auto & equipment = entity.addComponent<Equipment>(vfxManager);

    for (const auto & enginePrototype : m_engines)
    {
        auto engine = std::make_shared<EngineSlot>(enginePrototype.voxel, enginePrototype.pose);
        equipment.addEngineSlot(engine);
    }
    for (const auto & hardpointPrototype : m_hardpoints)
    {
        auto hardpoint = std::make_shared<Hardpoint>(hardpointPrototype.voxel,
                                                     hardpointPrototype.pose,
                                                     hardpointPrototype.maxAngle);
        equipment.addHardpoint(hardpoint);
    }
}

void EquipmentPrototype::loadSlotPrototype(const nlohmann::json & obj, SlotPrototype & slot) const
{
    slot.voxel = obj["Voxel"];
    std::swap(slot.voxel.y, slot.voxel.z);

    slot.pose.setOrientation(obj["Orientation"]);
}