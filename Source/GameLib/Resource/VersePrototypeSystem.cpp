#include "VersePrototypeSystem.h"

#include <Deliberation/ImGui/ImGuiSystem.h>

constexpr const char * RELOAD_PROTOTYPES_CONTROL = "Reload Prototypes";

VersePrototypeSystem::VersePrototypeSystem(World & world):
    Base(world),
    m_manager(std::make_shared<VersePrototypeManager>(world, GameDataPath("Data/Prototypes/list.json")))
{
    m_manager->reloadList();
}

void VersePrototypeSystem::onCreated()
{
    auto & imGuiSystem = world().systemRef<ImGuiSystem>();
    imGuiSystem.addControlItem(RELOAD_PROTOTYPES_CONTROL, [&]()
    {
        m_manager->reloadList();
        m_manager->updateEntities();
    });
}

void VersePrototypeSystem::onRemoved()
{
    auto & imGuiSystem = world().systemRef<ImGuiSystem>();
    imGuiSystem.removeControlItem(RELOAD_PROTOTYPES_CONTROL);
}