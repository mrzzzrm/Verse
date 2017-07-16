#pragma once

#include <Deliberation/ECS/Component.h>

#include "GameLib.h"

class NpcBehaviour : public Component<NpcBehaviour>
{
    DELIBERATION_COMPONENT_NAME("NpcBehaviour")
};