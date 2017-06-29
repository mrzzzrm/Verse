#pragma once

#include <Deliberation/Core/Json.h>
#include <Deliberation/ECS/ComponentPrototype.h>

#include "GameLib.h"
#include "NpcController.h"

class NpcControllerPrototype:
    public ComponentPrototype<NpcController>
{
public:
    void updateComponent(NpcController & npcController) override;
};
