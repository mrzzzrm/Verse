#pragma once

#include <memory>

#include "Engine.h"
#include "GameLib.h"
#include "ItemSlot.h"

struct EngineSlotDesc:
    ItemSlotDesc
{};

class EngineSlot:
    public ItemSlot
{
public:
    EngineSlot(const EngineSlotDesc & desc);

    const std::shared_ptr<Engine> & engine() const;
    void setEngine(const std::shared_ptr<Engine> & engine);

    void setTargetPose(const Pose3D & pose);

    void setVfxManager(VfxManager & vfxManager);

private:
    void onDisabled() override;

private:
    std::shared_ptr<Engine> m_engine;
    VfxManager *            m_vfxManager = nullptr;
};
