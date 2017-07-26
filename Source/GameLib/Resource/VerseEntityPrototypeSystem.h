#pragma once

#include <Deliberation/ECS/EntityPrototypeManager.h>
#include <Deliberation/ECS/EntityPrototypeSystem.h>
#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/World.h>

#include "GameLib.h"

class VerseEntityPrototypeSystem : public EntityPrototypeSystem
{
public:
    VerseEntityPrototypeSystem(World & world);

    void onCreated() override;
    void onRemoved() override;
};