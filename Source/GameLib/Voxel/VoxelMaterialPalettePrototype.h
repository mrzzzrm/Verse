#pragma once

#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <Deliberation/Resource/AbstractPrototype.h>

#include "GameLib.h"
#include "VoxelMaterialPrototype.h"

class VoxelMaterialPalettePrototype:
    public AbstractPrototype
{
public:
    explicit VoxelMaterialPalettePrototype(const std::weak_ptr<PrototypeManager> &prototypeManager):
        AbstractPrototype(prototypeManager)
    {}

    const std::unordered_map<glm::u8vec3, std::shared_ptr<const VoxelMaterialPrototype>> & materialByPaletteColor() const {
        return m_materialByPaletteColor;
    };

protected:
    void onReload(const Json &json) override;

private:
    std::unordered_map<glm::u8vec3, std::shared_ptr<const VoxelMaterialPrototype>>
        m_materialByPaletteColor;
};
