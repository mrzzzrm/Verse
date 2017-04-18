#include "EntityDesc.h"

#include <Deliberation/Core/StringUtils.h>

EntityDesc::EntityDesc(const std::vector<std::string> & prototypeNames):
    prototypeNames(prototypeNames)
{
    entityName = StringJoin(prototypeNames, "-");
}

EntityDesc::EntityDesc(const std::vector<std::string> & prototypeNames, const std::string & entityName):
    prototypeNames(prototypeNames),
    entityName(entityName)
{

}