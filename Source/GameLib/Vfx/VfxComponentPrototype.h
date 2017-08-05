#pragma once

#include <Deliberation/ECS/ComponentPrototype.h>

#include "GameLib.h"
#include "VfxComponent.h"

class VfxComponentPrototype:
    public ComponentPrototype<VfxComponent>
{
public:
    void updateComponent(VfxComponent & vfxComponent) override;
};