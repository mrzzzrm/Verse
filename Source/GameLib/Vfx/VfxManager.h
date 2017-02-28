#pragma once

#include <Deliberation/Core/Optional.h>

#include "VfxParticle.h"
#include "VfxRenderer.h"

class VoxelWorld;

class VfxManager final
{
public:
    VfxManager(
        Context & context,
        const Camera3D & camera,
        VoxelWorld & voxelWorld);

    VfxRenderer & renderer();
    const VfxRenderer & renderer() const;

    VfxMeshId baseParticleMeshId() const;

    VfxParticleId addParticle(VfxParticle particle);
    void removeParticle(VfxParticleId particle);

    void update(float seconds);
    void render();

private:
    VoxelWorld &  m_voxelWorld;
    VfxRenderer   m_renderer;

    VfxMeshId     m_baseParticleMeshId = 0;
};
