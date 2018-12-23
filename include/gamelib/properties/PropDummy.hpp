#ifndef GAMELIB_PROPDUMMY_HPP
#define GAMELIB_PROPDUMMY_HPP

#include "PropType.hpp"

/*
 * Does nothing.
 * Can be used to hide log entries saying the property has no effect, e.g.
 * when there are json entries that must be loaded manually.
 */

namespace gamelib
{
    class PropertyContainer;

    class PropDummy : public Identifier<invalidID, IPropType>
    {
        public:
            inline bool loadFromJson(const PropertyHandle&, const Json::Value&) const { return true; };
            inline void writeToJson(const PropertyHandle&, Json::Value&)        const { };
            inline bool drawGui(const PropertyHandle&, const std::string&)      const { return false; };
    };

    extern PropDummy propDummy;

    void registerDummyProperty(PropertyContainer& props, const std::string& name);
}

#endif
