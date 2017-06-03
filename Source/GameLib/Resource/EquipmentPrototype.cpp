#include "EquipmentPrototype.h"

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Json.h>
#include <Deliberation/Core/StreamUtils.h>

#include "Equipment.h"
#include "Hardpoint.h"

EquipmentPrototype::EquipmentPrototype(const Json & json, VfxManager & vfxManager):
    m_vfxManager(vfxManager)
{
    for (const auto & obj : json["Engines"])
    {
        EngineSlotDesc desc;
        loadSlotDesc(obj, desc);

        m_desc.engineSlotDescs.emplace_back(desc);
    }

    for (const auto & obj : json["Hardpoints"])
    {
        HardpointDesc desc;
        loadSlotDesc(obj, desc);

        desc.maxAngle = obj["MaxAngle"];

        m_desc.hardpointDescs.emplace_back(desc);
    }

    if (json.count("VoxelLights") > 0)
    {
        for (const auto & obj : json["VoxelLights"])
        {
            VoxelLightDesc desc;
            loadSlotDesc(obj, desc);

            desc.intensity = obj["Intensity"];

            m_desc.voxelLightDescs.emplace_back(desc);
        }
    }
}

void EquipmentPrototype::applyToEntity(Entity & entity) const
{
    auto & equipment = entity.addComponent<Equipment>(m_vfxManager, m_desc);
}

void EquipmentPrototype::loadSlotDesc(const Json & obj, AttachmentDesc & slot) const
{
    slot.voxel = obj["Voxel"];
    std::swap(slot.voxel.y, slot.voxel.z);

    slot.pose.setPosition(obj.value("Position", glm::vec3()));
    slot.pose.setOrientation(glm::quat(obj.value<glm::quat>("Orientation", glm::quat())));
}