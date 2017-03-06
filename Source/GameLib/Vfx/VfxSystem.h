#pragma once

#include <memory>

#include <Deliberation/ECS/System.h>

#include "GameLib.h"

class Emitter;
class VfxManager;
class VoxelObjectModification;
class VoxelObjectBulletHit;

class VfxSystem final:
    public System<VfxSystem>
{
public:
    VfxSystem(World & world, VfxManager & vfxManager);

    void receive(const VoxelObjectModification & modification);
    void receive(const VoxelObjectBulletHit & hit);

private:
    VfxManager &                m_vfxManager;
    std::shared_ptr<Emitter>    m_blastEmitter;
    std::shared_ptr<Emitter>    m_smokeEmitter;
};