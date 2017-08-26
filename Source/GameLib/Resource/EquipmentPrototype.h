#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Deliberation/Core/Json.h>
#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/ECS/ComponentPrototype.h>
#include <Deliberation/ECS/Entity.h>

#include "Attachment.h"
#include "Equipment.h"
#include "GameLib.h"

class VfxManager;

class EquipmentPrototype final : public ComponentPrototype<Equipment>
{
public:
    EquipmentPrototype(const std::shared_ptr<VfxManager> & vfxManager)
        : m_vfxManager(vfxManager)
    {
    }

    void updateComponent(const Entity & entity, Equipment & equipment) override;

protected:
    void initComponent(const Entity & entity, Equipment & equipment) override;

private:
    void loadSlotDesc(const Json & obj, AttachmentDesc & slot) const;

private:
    std::shared_ptr<VfxManager> m_vfxManager;
};