#include "HudEntityMarkersRenderer.h"

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Resource/ResourceManager.h>

#include <Deliberation/Platform/App.h>
#include <Deliberation/Platform/AppRuntime.h>

#include "HudButton.h"

HudEntityMarkersRenderer::HudEntityMarkersRenderer()
{
    auto & resourceManager = *App::get().runtime()->resourceManager();

    auto mesh = resourceManager.mesh(R::HudEntityMarkerUpperLeft);
    auto program = resourceManager.program(R::HudElement);

    auto instanceLayout = DataLayout({{"ElementPosition", Type_Vec2},
                                      {"ElementColor", Type_Vec3},
                                      {"Flip", Type_Vec2}});

    m_instances = LayoutedBlob(instanceLayout);
    m_instanceBuffer = GetGlobal<DrawContext>()->createBuffer(instanceLayout);

    m_draw = GetGlobal<DrawContext>()->createDraw(program);
    m_draw.setIndices(mesh.indices());
    m_draw.addVertices(mesh.vertices());
    m_draw.addInstanceBuffer(m_instanceBuffer, 1);
    m_draw.sampler("Texture").setTexture(mesh.textures()[0]);
    m_draw.state().setDepthState(DepthState::disabledR());
    m_draw.state().setCullState(CullState::disabled());
    m_draw.state().setBlendState({BlendEquation::Add,
                                  BlendFactor::SourceAlpha,
                                  BlendFactor::OneMinusSourceAlpha});
    m_viewportSizeUniform = m_draw.uniform("ViewportSize");
}

void HudEntityMarkersRenderer::render(
    const std::vector<std::shared_ptr<HudButton>> & markers)
{
    m_instances.resize(markers.size() * 4); // 4 corners per marker

    auto elementPositions = m_instances.iterator<glm::vec2>("ElementPosition");
    auto elementColors = m_instances.iterator<glm::vec3>("ElementColor");
    auto flips = m_instances.iterator<glm::vec2>("Flip");

    for (const auto & marker : markers)
    {
        const auto x = marker->halfExtent().x;
        const auto y = marker->halfExtent().y;

        elementColors.put(marker->color());
        elementPositions.put(marker->position() + glm::vec2(-x, y));
        flips.put({1, 1});

        elementColors.put(marker->color());
        elementPositions.put(marker->position() + glm::vec2(x, y));
        flips.put({-1, 1});

        elementColors.put(marker->color());
        elementPositions.put(marker->position() + glm::vec2(x, -y));
        flips.put({-1, -1});

        elementColors.put(marker->color());
        elementPositions.put(marker->position() + glm::vec2(-x, -y));
        flips.put({1, -1});
    }

    m_instanceBuffer.upload(m_instances);

    m_viewportSizeUniform.set(glm::vec2{GetGlobal<DrawContext>()->backbuffer().width(),
                                        GetGlobal<DrawContext>()->backbuffer().height()});

    m_draw.render();
}