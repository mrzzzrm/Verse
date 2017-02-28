#include "VfxManager.h"

#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/Texture.h>
#include <Deliberation/Draw/TextureLoader.h>

VfxManager::VfxManager(
    Context & context,
    const Camera3D & camera,
    VoxelWorld & voxelWorld
):
    m_voxelWorld(voxelWorld),
    m_renderer(context, camera)
{
    /**
     * Init base particle got nowhere else to put this right now
     */
    const auto layout = DataLayout{{
        {"Position", Type_Vec3},
        {"UV", Type_Vec2}
    }};

    LayoutedBlob vertices(layout, 4);
    auto positions = vertices.field<glm::vec3>("Position");
    auto uvs = vertices.field<glm::vec2>("UV");

    positions.assign({
         glm::vec3(-0.5f, -0.5f, 0.0f),
         glm::vec3(0.5f, -0.5f, 0.0f),
         glm::vec3(0.5f, 0.5f, 0.0f),
         glm::vec3(-0.5f, 0.5f, 0.0f)
    });

    uvs.assign({
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f)
    });

    LayoutedBlob indicesBlob(DataLayout("Index", Type_U32), 6);
    auto indices = indicesBlob.field<u32>("Index");

    indices.assign({0, 1, 3, 1, 2, 3});

    const auto textureBinary = TextureLoader("Data/Particles/BaseParticle.png").load();
    const auto texture = context.createTexture(textureBinary);

    Mesh2 mesh(std::move(vertices), std::move(indicesBlob), {texture});

    m_baseParticleMeshId = m_renderer.addMesh(std::move(mesh));
}

VfxRenderer & VfxManager::renderer()
{
    return m_renderer;
}

const VfxRenderer & VfxManager::renderer() const
{
    return m_renderer;
}

VfxMeshId VfxManager::baseParticleMeshId() const
{
    return m_baseParticleMeshId;
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
