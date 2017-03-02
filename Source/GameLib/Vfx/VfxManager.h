#pragma once

#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/SparseVector.h>

#include "EmitterInstance.h"
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

    void addEmitterInstance(std::shared_ptr<EmitterInstance> emitterInstance);

    void update(float seconds);
    void render();

private:
    VoxelWorld &                    m_voxelWorld;
    VfxRenderer                     m_renderer;

    VfxMeshId                       m_baseParticleMeshId = 0;

    SparseVector<std::shared_ptr<EmitterInstance>>
                                    m_emitterInstances;
};
