#include "VerseEntityPrototypeSystem.h"

#include <Deliberation/ImGui/ImGuiSystem.h>

constexpr const char * RELOAD_PROTOTYPES_CONTROL = "Reload Prototypes";

VerseEntityPrototypeSystem::VerseEntityPrototypeSystem(World & world)
    : Base(world)
    , m_manager(std::make_shared<VerseEntityPrototypeManager>(
          world, GameDataPath("Data/EntityPrototypes/list.json")))
{
    m_manager->reloadList();
}

void VerseEntityPrototypeSystem::onCreated()
{
    auto & imGuiSystem = world().systemRef<ImGuiSystem>();
    imGuiSystem.addControlItem(RELOAD_PROTOTYPES_CONTROL, [&]() {
        m_manager->reloadList();
        m_manager->updateEntities();
    });
}

void VerseEntityPrototypeSystem::onRemoved()
{
    auto & imGuiSystem = world().systemRef<ImGuiSystem>();
    imGuiSystem.removeControlItem(RELOAD_PROTOTYPES_CONTROL);
}