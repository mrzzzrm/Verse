#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "GameLib.h"

class EntityDesc final
{
public:
    EntityDesc(const std::vector<std::string> & prototypeNames);
    EntityDesc(const std::vector<std::string> & prototypeNames, const std::string & entityName);

    std::string                 entityName;
    std::vector<std::string>    prototypeNames;
    glm::vec3                   position;
};
