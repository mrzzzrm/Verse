#pragma once

#include <vector>

#include <Deliberation/Core/RandomColorGenerator.h>

#include <Deliberation/ECS/System.h>

#include <Deliberation/Scene/Debug/DebugGeometryNode.h>
#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>

#include "GameLib.h"

namespace deliberation
{
class Camera3D;
class DrawContext;
} // namespace deliberation

class NpcDebugRendererSystem : public System<NpcDebugRendererSystem>
{
public:
    NpcDebugRendererSystem(
        World & world, const Camera3D & camera);

protected:
    void onEntityAdded(Entity & entity) override;
    void onEntityRemoved(Entity & entity) override;

private:
    std::vector<Entity> m_entities;
    //    DebugGeometryRenderer    m_geometryManager;
    //    DebugGeometryNode   m_geometryRenderer;
    RandomColorGenerator m_randomColorGenerator;
    const Camera3D &     m_camera;
};