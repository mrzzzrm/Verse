#include "VoxelMaterialPalettePrototype.h"

#include <Deliberation/Resource/PrototypeManager.h>

void VoxelMaterialPalettePrototype::onReload(const Json &json)
{
    if (!json.is_array()) return;

    m_materialByPaletteColor.clear();

    for (const auto & materialMapping : json) {
        AssertM(materialMapping.size() == 2, "Invalid voxel material mapping json: \n" + materialMapping.dump() + "\n");

        const auto paletteColor = materialMapping[0].get<glm::u8vec3>();
        const auto materialName = materialMapping[1].get<std::string>();

        m_materialByPaletteColor.emplace(paletteColor,
                                         prototypeManagerRef().prototype<VoxelMaterialPrototype>(materialName));
    }
}