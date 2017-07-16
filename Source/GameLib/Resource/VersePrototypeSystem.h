#pragma once

#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/World.h>

#include "VersePrototypeManager.h"
#include "GameLib.h"

class VersePrototypeSystem:
    public System<VersePrototypeSystem>
{
public:
    VersePrototypeSystem(World & world);

    const std::shared_ptr<VersePrototypeManager> & manager() const { return m_manager; }

    void onCreated() override;
    void onRemoved() override;

private:
    std::shared_ptr<VersePrototypeManager> m_manager;
};