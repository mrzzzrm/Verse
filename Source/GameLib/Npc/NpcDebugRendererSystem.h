#pragma once

#include <vector>

#include <Deliberation/Core/RandomColorGenerator.h>

#include <Deliberation/ECS/System.h>

#include <Deliberation/Scene/Debug/DebugGeometryManager.h>
#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>

#include "GameLib.h"

namespace deliberation
{
class Camera3D;
class Context;
}

class NpcDebugRendererSystem:
    public System<NpcDebugRendererSystem>
{
public:
    NpcDebugRendererSystem(World & world, Context & context, const Camera3D & camera);

protected:
    void onEntityAdded(Entity & entity) override;
    void onEntityRemoved(Entity & entity) override;
    void onRender() override;

private:
    std::vector<Entity>     m_entities;
    DebugGeometryManager    m_geometryManager;
    DebugGeometryRenderer   m_geometryRenderer;
    RandomColorGenerator    m_randomColorGenerator;
    const Camera3D &        m_camera;
};