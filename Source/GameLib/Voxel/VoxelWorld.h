#pragma once

#include <memory>
#include <set>
#include <unordered_map>

#include <glm/glm.hpp>

#include <Deliberation/Draw/Program.h>
#include <Deliberation/Draw/Texture.h>

#include <Deliberation/ECS/System.h>

#include "VoxelClusterMarchingCubesTriangulation.h"
#include "VoxelDefines.h"
#include "VoxelObject.h"
#include "VoxelRenderer.h"

namespace deliberation
{
class Camera3D;
class DrawContext;
class PhysicsWorld;

} // namespace deliberation

class VoxelObject;
class VoxelObjectID;
class VoxelObjectModification;

class VoxelWorld final : public System<VoxelWorld>
{
public:
    VoxelWorld(World & world, const Texture & envMap);

    DrawContext & drawContext() const;
    const VoxelClusterMarchingCubesTriangulation &
                    marchingCubesTriangulation() const;
    const Program & program() const;
    const Texture & envMap() const;

    void onCrucialVoxelDestroyed(VoxelObject & voxelObject);

    void onEntityAdded(Entity & entity) override;
    void onEntityRemoved(Entity & entity) override;
    void onEntityGameUpdate(Entity & entity, const UpdateFrame & updateFrame) override;
    void onGameUpdate(const UpdateFrame & updateFrame) override;

private:
    std::shared_ptr<VoxelRenderer> m_renderer;

    DrawContext &                          m_drawContext;
    Texture                                m_envMap;
    VoxelClusterMarchingCubesTriangulation m_marchingCubesTriangulation;
    Program                                m_program;
};