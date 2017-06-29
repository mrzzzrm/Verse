#include "EquipmentPrototype.h"

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Json.h>
#include <Deliberation/Core/StreamUtils.h>

#include "Equipment.h"
#include "Hardpoint.h"

void EquipmentPrototype::updateComponent(Equipment & equipment)
{
    /**
     * Clear Equipment
     */
    equipment.m_hardpoints.clear();
    equipment.m_engineSlots.clear();
    equipment.m_attachmentByVoxel.clear();

    if (m_json.count("Engines") > 0) {
        for (const auto &obj : m_json["Engines"]) {
            EngineSlotDesc desc;
            loadSlotDesc(obj, desc);

            auto engineSlot = std::make_shared<EngineSlot>(desc);
            engineSlot->setVfxManager(m_vfxManager);
            equipment.m_engineSlots.emplace_back(engineSlot);
            equipment.addAttachment(engineSlot);
        }
    }

    if (m_json.count("Hardpoints") > 0) {
        for (const auto &obj : m_json["Hardpoints"]) {
            HardpointDesc desc;
            loadSlotDesc(obj, desc);

            desc.maxAngle = obj["MaxAngle"];

            auto hardpoint = std::make_shared<Hardpoint>(desc);
            equipment.m_hardpoints.emplace_back(hardpoint);
            equipment.addAttachment(hardpoint);
        }
    }

    if (m_json.count("VoxelLights") > 0)
    {
        for (const auto & obj : m_json["VoxelLights"])
        {
            VoxelLightDesc desc;
            loadSlotDesc(obj, desc);

            desc.intensity = obj["Intensity"];

            auto voxelLight = std::make_shared<VoxelLight>(desc);
            equipment.addAttachment(voxelLight);
        }
    }
}

void EquipmentPrototype::initComponent(Equipment & equipment)
{
    equipment.setVfxManager(m_vfxManager);
}

void EquipmentPrototype::loadSlotDesc(const Json & obj, AttachmentDesc & slot) const
{
    slot.voxel = obj["Voxel"];
    std::swap(slot.voxel.y, slot.voxel.z);

    slot.pose.setPosition(obj.value("Position", glm::vec3()));
    slot.pose.setOrientation(glm::quat(obj.value<glm::quat>("Orientation", glm::quat())));
}