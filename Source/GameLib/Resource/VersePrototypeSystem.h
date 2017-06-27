#pragma once

#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/World.h>

#include "VersePrototypeManager.h"
#include "GameLib.h"

class VersePrototypeSystem:
    public System<VersePrototypeSystem>
{
public:
    VersePrototypeSystem(World & world):
        Base(world),
        m_manager(std::make_shared<VersePrototypeManager>(world, GameDataPath("Data/Prototypes/list.json")))
    {
        m_manager->reloadList();
    }

    const std::shared_ptr<VersePrototypeManager> & manager() const { return m_manager; }

private:
    std::shared_ptr<VersePrototypeManager> m_manager;
};