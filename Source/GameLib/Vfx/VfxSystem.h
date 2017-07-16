#pragma once

#include <memory>

#include <Deliberation/ECS/System.h>

#include "GameLib.h"
#include "VfxManager.h"

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
    void onEvent(const VoxelObjectBulletHit & hit);

protected:
    void onGameUpdate(float seconds) override;

private:
    std::shared_ptr<VfxManager> m_vfxManager;
    std::shared_ptr<Emitter>    m_blastEmitter;
    std::shared_ptr<Emitter>    m_smokeEmitter;
};