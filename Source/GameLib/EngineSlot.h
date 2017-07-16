#pragma once

#include <memory>

#include "Attachment.h"
#include "Engine.h"
#include "GameLib.h"

struct EngineSlotDesc : AttachmentDesc
{
};

class EngineSlot : public Attachment
{
public:
    EngineSlot(const EngineSlotDesc & desc);

    const std::shared_ptr<Engine> & engine() const;
    void setEngine(const std::shared_ptr<Engine> & engine);

    void setTargetPose(const Pose3D & pose);

    void setVfxManager(const std::shared_ptr<VfxManager> & vfxManager)
    {
        m_vfxManager = vfxManager;
    }

private:
    void onDisabled() override;

private:
    std::shared_ptr<Engine>     m_engine;
    std::shared_ptr<VfxManager> m_vfxManager;
};
