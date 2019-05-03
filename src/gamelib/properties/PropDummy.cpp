#include "gamelib/properties/PropDummy.hpp"
#include "gamelib/properties/PropertyContainer.hpp"

namespace gamelib
{
    PropDummy propDummy;

    void registerDummyProperty(PropertyContainer& props, const std::string& name)
    {
        props.registerProperty(name, propDummy, &propDummy);
    }
}
