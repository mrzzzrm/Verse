#pragma once

#include <unordered_map>

#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/EntityPrototypeManager.h>
#include <Deliberation/ECS/World.h>

#include "GameLib.h"

class EntityPrototype;

class VerseEntityPrototypeManager : public EntityPrototypeManager
{
public:
    VerseEntityPrototypeManager(World & world, const std::string & listPath);
};