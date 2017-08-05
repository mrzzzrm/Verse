#include "EngineSlot.h"

#include <Deliberation/Physics/RigidBody.h>

#include "Equipment.h"
#include "EquipmentUpdateContext.h"
#include "VfxManager.h"

EngineSlot::EngineSlot(const EngineSlotDesc & desc) : Attachment(desc) {}

const std::shared_ptr<Engine> & EngineSlot::engine() const { return m_engine; }

void EngineSlot::setEngine(const std::shared_ptr<Engine> & engine)
{
    m_engine = engine;

    auto & emitterInstance = m_engine->emitterInstance();

    m_vfxManager->addEmitterInstance(emitterInstance);
}

void EngineSlot::setTargetPose(const Pose3D & pose)
{
    if (m_engine)
    {
        m_engine->emitterInstance()->setTargetPose(worldPose());
    }
}

void EngineSlot::onDisabled()
{
    if (!m_engine) return;

    m_vfxManager->removeEmitterInstance(m_engine->emitterInstance());
}

void EngineSlot::onGameUpdate(float seconds, const EquipmentUpdateContext & context)
{
    if (m_engine)
    {
        m_engine->emitterInstance()->setVelocity(context.body->linearVelocity());
    }
};