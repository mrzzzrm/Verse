#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <json.hpp>

#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/ECS/Entity.h>

#include "GameLib.h"

class VfxManager;

struct SlotPrototype
{
    glm::uvec3  voxel;
    Pose3D      pose;
};

struct EnginePrototype:
    SlotPrototype
{};

struct HardpointPrototype:
    SlotPrototype
{
    float maxAngle = 0.0f;
};

class EquipmentPrototype final
{
public:
    EquipmentPrototype(nlohmann::json & json);

    const std::vector<EnginePrototype> & engines() const;
    const std::vector<HardpointPrototype> & hardpoints() const;

    void applyToEntity(Entity & entity, VfxManager & vfxManager) const;

private:
    void loadSlotPrototype(const nlohmann::json & obj, SlotPrototype & slot) const;

private:
    std::vector<EnginePrototype>    m_engines;
    std::vector<HardpointPrototype> m_hardpoints;
};