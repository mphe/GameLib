#ifndef GAMELIB_JSON_NATIVES_HPP
#define GAMELIB_JSON_NATIVES_HPP

#include "json/json.h"

// This file contains overloads for basic json types (int, float, string, ...)

#define _JSON_BASIC_OVERLOAD(type, name) \
    inline bool loadFromJson(const Json::Value& node, type& obj)  \
    {   \
        obj = node.as##name(); \
        return true;    \
    }   \
    inline void writeToJson(Json::Value& node, const type& obj)  \
    {   \
        node = obj; \
    }

namespace gamelib
{
    _JSON_BASIC_OVERLOAD(int, Int)
    _JSON_BASIC_OVERLOAD(unsigned int, UInt)
    _JSON_BASIC_OVERLOAD(bool, Bool)
    _JSON_BASIC_OVERLOAD(float, Float)
    _JSON_BASIC_OVERLOAD(double, Double)
    _JSON_BASIC_OVERLOAD(std::string, String)
}

#undef _LOAD_BASIC_OVERLOAD

#endif
