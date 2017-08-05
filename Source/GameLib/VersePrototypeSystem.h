#pragma once

#include <Deliberation/Resource/PrototypeSystem.h>

#include "GameLib.h"

class VersePrototypeSystem:
    public PrototypeSystem
{
public:
    VersePrototypeSystem(World & world);

    void reload();

    void onCreated() override;
};

