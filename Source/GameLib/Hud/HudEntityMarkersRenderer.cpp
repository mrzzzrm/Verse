#include "HudEntityMarkersRenderer.h"

#include <Deliberation/Draw/Context.h>

#include "HudButton.h"
#include "R.h"
#include "ResourceManager.h"

HudEntityMarkersRenderer::HudEntityMarkersRenderer(Context & context, ResourceManager & resourceManager):
    m_context(context)
{
    auto mesh = resourceManager.mesh(R::HudEntityMarkerUpperLeft);
    auto program = resourceManager.program(R::HudElement);

    auto instanceLayout = DataLayout({{"ElementPosition", Type_Vec2}, {"Flip", Type_Vec2}});

    m_instances = LayoutedBlob(instanceLayout);
    m_instanceBuffer = context.createBuffer(instanceLayout);

    m_draw = context.createDraw(program);
    m_draw.setIndices(mesh.indices());
    m_draw.addVertices(mesh.vertices());
    m_draw.addInstanceBuffer(m_instanceBuffer, 1);
    m_draw.sampler("Texture").setTexture(mesh.textures()[0]);
    m_draw.state().setDepthState(DepthState::disabledR());
    m_draw.state().setCullState(CullState::disabled());
    m_draw.state().setBlendState({gl::GL_FUNC_ADD, gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA});
    m_viewportSizeUniform = m_draw.uniform("ViewportSize");
}

void HudEntityMarkersRenderer::render(const std::vector<std::shared_ptr<HudButton>> & markers)
{
    std::cout << "Markers: " << markers.size() << std::endl;

    m_instances.resize(markers.size() * 4); // 4 corners per marker

    m_elementPositions = m_instances.iterator<glm::vec2>("ElementPosition");
    m_flips = m_instances.iterator<glm::vec2>("Flip");
    
    for (const auto & marker : markers)
    {
        const auto x = marker->halfExtent().x;
        const auto y = marker->halfExtent().y;

        m_elementPositions.put(marker->position() + glm::vec2(-x, y));
        m_flips.put({1, 1});
        m_elementPositions.put(marker->position() + glm::vec2(x, y));
        m_flips.put({-1, 1});
        m_elementPositions.put(marker->position() + glm::vec2(x, -y));
        m_flips.put({-1, -1});
        m_elementPositions.put(marker->position() + glm::vec2(-x, -y));
        m_flips.put({1, -1});
    }
    
    m_instanceBuffer.scheduleUpload(m_instances);

    m_viewportSizeUniform.set(glm::vec2{m_context.backbuffer().width(), m_context.backbuffer().height()});

    m_draw.schedule();
}