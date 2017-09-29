#pragma once

#include <Deliberation/Scene/Pipeline/SingleNodeRenderer.h>

#include "GameLib.h"

namespace deliberation
{
class DebugGeometryNode;
}

class VfxManager;

class VfxDebugRenderer final
{
public:
    VfxDebugRenderer(const std::shared_ptr<VfxManager> & vfxManager);

    void setVisible(bool visible);

    void update();

private:
    std::shared_ptr<VfxManager> m_vfxManager;
    std::shared_ptr<DebugGeometryNode> m_debugGeometryNode;
};