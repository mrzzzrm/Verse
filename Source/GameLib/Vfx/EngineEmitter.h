#pragma once

#include <vector>

#include "HailstormDefines.h"
#include "HailstormManager.h"
#include "HailstormParticle.h"

class EngineEmitter final
{
public:
    EngineEmitter(HailstormManager & hailstormManager, const std::vector<HailstormMeshID> & meshIDs);

    void update(float seconds);

private:
    HailstormManager &                      m_hailstormManager;
    const std::vector<HailstormMeshID> &    m_meshIDs;
};