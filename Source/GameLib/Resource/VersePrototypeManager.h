#pragma once

#include <unordered_map>

#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/PrototypeManager.h>
#include <Deliberation/ECS/World.h>

#include "GameLib.h"

class EntityPrototype;

class VersePrototypeManager : public PrototypeManager
{
public:
    VersePrototypeManager(World & world, const std::string & listPath);
};