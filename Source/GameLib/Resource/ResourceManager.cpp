#include "ResourceManager.h"

#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>

#include <Deliberation/ECS/Systems/ApplicationSystem.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/MeshData.h>
#include <Deliberation/Scene/Texture/TextureLoader.h>
#include <Deliberation/Scene/UVSphere.h>

ResourceManager::ResourceManager(World & world)
    : Base(world)
    , m_drawContext(world.systemRef<ApplicationSystem>().drawContext())
{

}

void ResourceManager::registerBuildIns()
{
    /**
     * Init base particle got nowhere else to put this right now
     */
    {
        const auto layout =
            DataLayout{{{"Position", Type_Vec3}, {"UV", Type_Vec2}}};

        LayoutedBlob vertices(layout, 4);
        auto         positions = vertices.field<glm::vec3>("Position");
        auto         uvs = vertices.field<glm::vec2>("UV");

        positions.assign({glm::vec3(-0.5f, -0.5f, 0.0f),
                          glm::vec3(0.5f, -0.5f, 0.0f),
                          glm::vec3(0.5f, 0.5f, 0.0f),
                          glm::vec3(-0.5f, 0.5f, 0.0f)});

        uvs.assign({glm::vec2(0.0f, 0.0f),
                    glm::vec2(1.0f, 0.0f),
                    glm::vec2(1.0f, 1.0f),
                    glm::vec2(0.0f, 1.0f)});

        LayoutedBlob indicesBlob(DataLayout("Index", Type_U32), 6);
        auto         indices = indicesBlob.field<u32>("Index");

        indices.assign({0, 1, 3, 1, 2, 3});

        const auto textureBinary =
            TextureLoader(GameDataPath("Data/Particles/BaseParticle.png"))
                .load();
        const auto texture = m_drawContext.createTexture(textureBinary);

        auto meshData = std::make_shared<MeshData>(std::move(vertices), std::move(indicesBlob), std::vector<Texture>{texture});

        addResource<std::shared_ptr<MeshData>>("buildin:BaseParticle", std::move(meshData));
    }

    /**
     * Init bullet particle
     */
    {
        auto meshData = std::make_shared<MeshData>(UVSphere(5, 5).generateMesh2());
        addResource<std::shared_ptr<MeshData>>("buildin:Bullet", std::move(meshData));
    }

    /**
     * Init crosshair
     */
    {
        const auto layout =
            DataLayout{{{"Position", Type_Vec2}, {"UV", Type_Vec2}}};

        LayoutedBlob vertices(layout, 4);
        auto         positions = vertices.field<glm::vec2>("Position");
        auto         uvs = vertices.field<glm::vec2>("UV");

        positions.assign({glm::vec2(-0.5f, -0.5f),
                          glm::vec2(0.5f, -0.5f),
                          glm::vec2(0.5f, 0.5f),
                          glm::vec2(-0.5f, 0.5f)});

        uvs.assign({glm::vec2(0.0f, 0.0f),
                    glm::vec2(1.0f, 0.0f),
                    glm::vec2(1.0f, 1.0f),
                    glm::vec2(0.0f, 1.0f)});

        LayoutedBlob indicesBlob(DataLayout("Index", Type_U32), 6);
        auto         indices = indicesBlob.field<u32>("Index");

        indices.assign({0, 1, 3, 1, 2, 3});

        const auto textureBinary =
            TextureLoader(GameDataPath("Data/Ui/Crosshairs.png")).load();
        const auto texture = m_drawContext.createTexture(textureBinary);

        MeshData mesh(std::move(vertices), std::move(indicesBlob), {texture});
        m_meshByResourceId.emplace(
            (size_t)R::UiCrosshairMesh, std::make_shared<MeshData>(mesh));
    }

    /**
     * Init HudEntityMarkerUpperLeft
     */
    {
        const auto layout =
            DataLayout{{{"Position", Type_Vec2}, {"UV", Type_Vec2}}};

        LayoutedBlob vertices(layout, 4);
        auto         positions = vertices.field<glm::vec2>("Position");
        auto         uvs = vertices.field<glm::vec2>("UV");

        positions.assign({glm::vec2(-0.5f, -0.5f),
                          glm::vec2(0.5f, -0.5f),
                          glm::vec2(0.5f, 0.5f),
                          glm::vec2(-0.5f, 0.5f)});

        uvs.assign({glm::vec2(0.0f, 0.0f),
                    glm::vec2(1.0f, 0.0f),
                    glm::vec2(1.0f, 1.0f),
                    glm::vec2(0.0f, 1.0f)});

        LayoutedBlob indicesBlob(DataLayout("Index", Type_U32), 6);
        auto         indices = indicesBlob.field<u32>("Index");

        indices.assign({0, 1, 3, 1, 2, 3});

        const auto textureBinary =
            TextureLoader(GameDataPath("Data/Ui/HudEntityMarkerUpperLeft.png"))
                .load();
        const auto texture = m_drawContext.createTexture(textureBinary);

        MeshData mesh(std::move(vertices), std::move(indicesBlob), {texture});
        m_meshByResourceId.emplace(
            (size_t)R::HudEntityMarkerUpperLeft,
            std::make_shared<MeshData>(mesh));
    }

    auto & context = m_world.systemRef<ApplicationSystem>().drawContext();

    {
        auto program = m_drawContext.createProgram(
            {GameDataPath("Data/Shaders/HudElement.vert"),
             GameDataPath("Data/Shaders/HudElement.frag")});
        m_programByResourceId.emplace((size_t)R::HudElement, program);
    }
}

const MeshData & ResourceManager::mesh(::ResourceId resourceId) const
{
    const auto iter = m_meshByResourceId.find((size_t)resourceId);
    Assert(
        iter != m_meshByResourceId.end(),
        "Couldn't find resource " + std::to_string((size_t)resourceId));

    return *iter->second;
}

const Program & ResourceManager::program(::ResourceId resourceId) const
{
    const auto iter = m_programByResourceId.find((size_t)resourceId);
    Assert(
        iter != m_programByResourceId.end(),
        "Couldn't find resource " + std::to_string((size_t)resourceId));

    return iter->second;
}
