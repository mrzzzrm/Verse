#include "EquipmentPrototype.h"

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Json.h>
#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Resource/PrototypeSystem.h>

#include "Equipment.h"
#include "Hardpoint.h"
#include "HailstormManager.h"
#include "VersePrototypeSystem.h"

void EquipmentPrototype::updateComponent(const Entity & entity, Equipment & equipment)
{
    auto prototypeManager = world().systemRef<VersePrototypeSystem>().manager();

    /**
     * Clear Equipment
     */
    equipment.m_hardpoints.clear();
    equipment.m_engineSlots.clear();
    equipment.m_dockingPoints.clear();
    equipment.m_attachmentByVoxel.clear();

    if (m_newJson.count("Engines") > 0)
    {
        for (const auto & obj : m_newJson["Engines"])
        {
            EngineSlotDesc desc;
            loadSlotDesc(obj, desc);

            auto engineSlot = std::make_shared<EngineSlot>(desc);
            engineSlot->setVfxManager(m_vfxManager);
            equipment.m_engineSlots.emplace_back(engineSlot);
            equipment.addAttachment(engineSlot);

            const auto emitterName = obj.value("Emitter", "");
            if (!emitterName.empty())
            {
                auto emitter = prototypeManager->prototype<Emitter>(emitterName);
                engineSlot->setEngine(std::make_shared<Engine>(emitter));
            }
        }
    }

    if (m_newJson.count("Hardpoints") > 0)
    {
        for (const auto & obj : m_newJson["Hardpoints"])
        {
            HardpointDesc desc;
            loadSlotDesc(obj, desc);

            desc.maxAngle = obj["MaxAngle"];
            if (obj.count("CompatibleWeapons") > 0)
            {
                for (auto & weaponName : obj["CompatibleWeapons"])
                {
                    desc.compatibleWeapons.emplace_back(weaponName.get<std::string>());
                }
            }

            auto hardpoint = std::make_shared<Hardpoint>(desc);
            equipment.m_hardpoints.emplace_back(hardpoint);
            equipment.addAttachment(hardpoint);

            if (desc.compatibleWeapons.size() > 0)
            {
                auto weaponPrototype =
                    world().systemRef<PrototypeSystem>().manager()->prototype<WeaponPrototype>(desc.compatibleWeapons[0]);
                auto & hailstormManager =
                    world().systemRef<HailstormManager>();
                Assert(static_cast<bool>(weaponPrototype), "");
                hardpoint->setWeapon(std::make_shared<Weapon>(weaponPrototype, hailstormManager));
            }
        }
    }

    if (m_newJson.count("VoxelLights") > 0)
    {
        for (const auto & obj : m_newJson["VoxelLights"])
        {
            VoxelLightDesc desc;
            loadSlotDesc(obj, desc);

            desc.intensity = obj["Intensity"];

            auto voxelLight = std::make_shared<VoxelLight>(desc);
            equipment.addAttachment(voxelLight);
        }
    }

    if (m_newJson.count("DockingPoints") > 0)
    {
        for (const auto & obj : m_newJson["DockingPoints"])
        {
            DockingPointDesc desc;
            loadSlotDesc(obj, desc);

            auto dockingPoint = std::make_shared<DockingPoint>(desc);
            equipment.addAttachment(dockingPoint);
            equipment.m_dockingPoints.emplace_back(dockingPoint);
        }
    }
}

void EquipmentPrototype::initComponent(const Entity & entity, Equipment & equipment)
{
    equipment.setVfxManager(m_vfxManager);
}

void EquipmentPrototype::loadSlotDesc(
    const Json & obj, AttachmentDesc & slot) const
{
    slot.voxel = obj["Voxel"];
    std::swap(slot.voxel.y, slot.voxel.z);

    slot.pose.setPosition(obj.value("Position", glm::vec3()));

    const auto orientationIter = obj.find("Orientation");
    if (orientationIter != obj.end())
    {
        slot.pose.setOrientation(*orientationIter);
    }
    else
    {
        const auto forwardIter = obj.find("Forward");
        const auto upIter = obj.find("Up");

        Assert(
            (forwardIter == obj.end()) == (upIter == obj.end()),
            "Either supply Forward AND Up, or neither");

        if (forwardIter != obj.end()) {
            glm::vec3 forward = *forwardIter;
            glm::vec3 up = *upIter;

            auto right = glm::cross(forward, up);

            slot.pose.setOrientation(glm::quat_cast(glm::mat3(right, up, forward)));
        }
    }
}