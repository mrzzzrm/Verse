#include "NpcDebugRendererSystem.h"

#include <algorithm>

#include "NpcController.h"

NpcDebugRendererSystem::NpcDebugRendererSystem(World & world, DrawContext & context, const Camera3D & camera):
    Base(world, ComponentFilter::requires<NpcController>()),
    m_geometryManager(context),
    m_geometryRenderer(m_geometryManager),
    m_camera(camera)
{

}

void NpcDebugRendererSystem::onEntityAdded(Entity & entity)
{
//    m_entities.emplace_back(entity);
//    if (m_entities.size() > m_geometryRenderer.numArrows()) {
//        m_geometryRenderer.addArrow({}, {}, m_randomColorGenerator.generate());
//    }
}

void NpcDebugRendererSystem::onEntityRemoved(Entity & entity)
{
    std::remove(m_entities.begin(), m_entities.end(), entity);
}

//void NpcDebugRendererSystem::onRender()
//{
//    for (size_t e = 0; e < m_entities.size(); e++)
//    {
//        auto & npcController = m_entities[e].component<NpcController>();
////        auto position = npcController.body()->transform().position();
//
////        m_geometryRenderer.arrow(e).setOrigin(position);
////        m_geometryRenderer.arrow(e).setDelta(npcController.steering().destination() - position);
//    }
//
////    m_geometryRenderer.schedule(m_camera);
//}