#ifndef GAMELIB_JSON_FILE_HPP
#define GAMELIB_JSON_FILE_HPP

#include "json/json.h"

namespace gamelib
{
    bool loadJsonFromFile(const std::string& fname, Json::Value& node);
    bool writeJsonToFile(const std::string& fname, const Json::Value& node);


    // The below functions assume there's a respective loadFromJson/writeToJson
    // overload for T
    template <typename T>
    bool loadFromFile(const std::string& fname, T& obj)
    {
        Json::Value json;
        if (!loadJsonFromFile(fname, json))
            return false;
        return loadFromJson(json, obj);
    }

    template <typename T>
    void writeToFile(const std::string& fname, const T& obj)
    {
        Json::Value json;
        writeToJson(json, obj);
        writeJsonToFile(fname, json);
    }
}

#endif
