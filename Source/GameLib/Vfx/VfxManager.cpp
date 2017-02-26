#include "VfxManager.h"

VfxManager::VfxManager(
    Context & context,
    const Camera3D & camera,
    VoxelWorld & voxelWorld
):
    m_voxelWorld(voxelWorld),
    m_renderer(context, camera)
{
}

VfxRenderer & VfxManager::renderer()
{
    return m_renderer;
}

const VfxRenderer & VfxManager::renderer() const
{
    return m_renderer;
}

VfxParticleId VfxManager::addParticle(VfxParticle particle)
{
    return m_renderer.addParticle(particle);
}

void VfxManager::removeParticle(VfxParticleId particle)
{
    m_renderer.removeParticle(particle);
}

void VfxManager::update(float seconds)
{

}

void VfxManager::render()
{
    m_renderer.render();
}
