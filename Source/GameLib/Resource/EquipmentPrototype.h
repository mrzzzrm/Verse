#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Deliberation/Core/Json.h>
#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/ECS/Entity.h>

#include "ComponentPrototype.h"
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

class EquipmentPrototype final:
    public ComponentPrototype
{
public:
    EquipmentPrototype(const Json & json, VfxManager & vfxManager);

    const std::vector<EnginePrototype> & engines() const;
    const std::vector<HardpointPrototype> & hardpoints() const;

    void applyToEntity(Entity & entity) const override;

private:
    void loadSlotPrototype(const Json & obj, SlotPrototype & slot) const;

private:
    VfxManager &                    m_vfxManager;
    std::vector<EnginePrototype>    m_engines;
    std::vector<HardpointPrototype> m_hardpoints;
};