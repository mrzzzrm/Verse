#pragma once

#include <memory>

#include <Deliberation/ECS/System.h>

#include "GameLib.h"
#include "VfxManager.h"
#include "VfxDebugRenderer.h"
#include "VoxelObjectBulletHit.h"

namespace deliberation
{
class Camera3D;
}

class Emitter;
class VfxManager;
class VoxelObjectModification;
class VoxelObjectBulletHit;

class VfxSystem final : public System<VfxSystem>
{
public:
    VfxSystem(World & world);

    const std::shared_ptr<VfxManager> & manager() const { return m_vfxManager; }

    void onEvent(const VoxelObjectModification & modification);
    void onEvent(const VoxelObjectBulletHit & event);

    void onCreated() override;

    void onGameUpdate(const UpdateFrame & updateFrame);

private:
    std::shared_ptr<VfxDebugRenderer>     m_debugRenderer;
    std::shared_ptr<VfxManager>         m_vfxManager;
};