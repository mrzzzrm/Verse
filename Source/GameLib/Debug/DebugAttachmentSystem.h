#pragma once

#include <Deliberation/ECS/System.h>

#include <Deliberation/Scene/Debug/DebugGeometryNode.h>
#include <Deliberation/Scene/Pipeline/RenderManager.h>

#include "GameLib.h"

namespace deliberation
{
class DebugGeometryNode;
}

class Attachment;

class DebugAttachmentSystem : public System<DebugAttachmentSystem>
{
public:
    DebugAttachmentSystem(World & world);

    bool enabled() const { return m_enabled; }

    void setEnabled(bool enabled) { m_enabled = enabled; }

    void onCreated() override;

protected:
    void onFrameUpdate(float seconds) override;

    void updateSphere(
        DebugSphereInstance &               sphere,
        const std::shared_ptr<Attachment> & attachment,
        const Transform3D &                 transform);

    void updatePose(
        DebugPoseInstance &                 pose,
        const std::shared_ptr<Attachment> & attachment,
        const Transform3D &                 transform);

private:
    bool                               m_enabled = false;
    std::shared_ptr<DebugGeometryNode> m_visibleNode;
    std::shared_ptr<DebugGeometryNode> m_obscuredNode;
    size_t                             m_numAllocatedPrimitives = 0;
};