#include "HailstormRenderer.h"

#include <Deliberation/Core/Assert.h>

HailstormRenderer::HailstormRenderer(Context & context, Camera3D & camera):
    m_context(context),
    m_camera(camera)
{

}

Context & HailstormRenderer::context() const
{
    return m_context;
}

MeshID HailstormRenderer::addMesh(const Mesh2 & mesh)
{
    m_batches.emplace_back(*this, mesh);
    return (MeshID)(m_batches.size() - 1);
}

void HailstormRenderer::addBullet(const HailstormBullet & bullet, MeshID meshID)
{
    Assert(meshID < m_batches.size(), "MeshID not registered");
    m_batches[meshID]->addInstance(bullet);
}

void HailstormRenderer::removeBullet(HailstormBulletID bulletID)
{

}

void HailstormRenderer::update(float seconds)
{

}