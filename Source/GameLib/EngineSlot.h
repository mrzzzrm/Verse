#pragma once

#include <memory>

#include "Attachment.h"
#include "Engine.h"
#include "GameLib.h"

class EquipmentUpdateContext;

struct EngineSlotDesc : AttachmentDesc
{
};

class EngineSlot : public Attachment
{
public:
    EngineSlot(const EngineSlotDesc & desc);

    const std::shared_ptr<Engine> & engine() const;
    void setEngine(const std::shared_ptr<Engine> & engine);

    void setVfxManager(const std::shared_ptr<VfxManager> & vfxManager)
    {
        m_vfxManager = vfxManager;
    }

    void onPostPhysicsUpdate(const UpdateFrame & updateFrame) override;
    void onGameUpdate(const UpdateFrame & updateFrame, const EquipmentUpdateContext & context) override;

private:
    void onDisabled() override;

private:
    std::shared_ptr<Engine>     m_engine;
    std::shared_ptr<VfxManager> m_vfxManager;
};
