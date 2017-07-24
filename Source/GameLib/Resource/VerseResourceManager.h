#pragma once

#include "ResourceManager.h"

class VerseResourceManager:
    public ResourceManager
{
public:
    VerseResourceManager(World & world);

    void onCreated() override;
};