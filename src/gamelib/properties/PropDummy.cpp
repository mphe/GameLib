#include "gamelib/properties/PropDummy.hpp"
#include "gamelib/properties/Property.hpp"

namespace gamelib
{
    PropDummy propDummy;

    void registerDummyProperty(PropertyContainer& props, const std::string& name)
    {
        props.registerProperty(name, propDummy, &propDummy);
    }
}
