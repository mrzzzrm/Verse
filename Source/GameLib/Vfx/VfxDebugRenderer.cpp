#include "VfxDebugRenderer.h"

#include <Deliberation/Scene/Debug/DebugGeometryNode.h>
#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>

#include "VfxManager.h"

VfxDebugRenderer::VfxDebugRenderer(RenderManager & renderManager, const std::shared_ptr<VfxManager> & vfxManager):
    m_vfxManager(vfxManager)
{
    m_debugGeometryNode = renderManager.renderer<DebugGeometryRenderer>()->addNode();
}

void VfxDebugRenderer::setVisible(bool visible)
{
    m_debugGeometryNode->setVisible(visible);
}

void VfxDebugRenderer::update()
{
    m_debugGeometryNode->beginPrimitives();

    for (const auto & emitterInstance : m_vfxManager->emitterInstances())
    {
        m_debugGeometryNode->addPose(emitterInstance->basePose());
        m_debugGeometryNode->addArrow(emitterInstance->basePose().position(),
                                      emitterInstance->basePose().orientation() * glm::vec3(0.0f, 0.0f, -10.0f),
                                      glm::vec3(0.3f, 0.9f, 0.2f));
    }

    m_debugGeometryNode->endPrimitives();
}