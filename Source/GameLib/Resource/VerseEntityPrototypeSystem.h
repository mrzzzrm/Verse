#pragma once

#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/World.h>

#include "GameLib.h"
#include "VerseEntityPrototypeManager.h"

class VerseEntityPrototypeSystem : public System<VerseEntityPrototypeSystem>
{
public:
    VerseEntityPrototypeSystem(World & world);

    const std::shared_ptr<VerseEntityPrototypeManager> & manager() const
    {
        return m_manager;
    }

    void onCreated() override;
    void onRemoved() override;

private:
    std::shared_ptr<VerseEntityPrototypeManager> m_manager;
};