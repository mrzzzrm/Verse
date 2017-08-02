#pragma once

#include <Deliberation/Resource/AbstractPrototype.h>

#include "GameLib.h"

class VoxelMaterialPrototype:
    public AbstractPrototype
{
public:
    VoxelMaterialPrototype(const std::weak_ptr<PrototypeManager> &prototypeManager):
        AbstractPrototype(prototypeManager)
    {}

    float brightnessScale() const { return m_brightnessScale; }

protected:
    void onReload(const Json &json) override;

private:
    float m_brightnessScale = 1.0f;
};