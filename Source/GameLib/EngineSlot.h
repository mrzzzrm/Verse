#pragma once

#include <memory>

#include "Engine.h"
#include "GameLib.h"
#include "ItemSlot.h"

class EngineSlot:
    public ItemSlot
{
public:
    EngineSlot(const glm::uvec3 & voxel, const Pose3D & pose);

    const std::shared_ptr<Engine> & engine() const;
    void setEngine(const std::shared_ptr<Engine> & engine);

    void setTargetPose(const Pose3D & pose);

    void disable();

private:
    std::shared_ptr<Engine> m_engine;
};
