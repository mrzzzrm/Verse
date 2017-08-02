#include "VoxelMaterialPrototype.h"

void VoxelMaterialPrototype::onReload(const Json &json)
{
    m_brightnessScale = json["BrightnessScale"];
}