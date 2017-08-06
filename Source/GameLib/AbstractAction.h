#pragma once

#include "GameLib.h"

namespace deliberation
{
class World;
}

class AbstractAction
{
public:
    virtual void performAction(World & world) = 0;
};