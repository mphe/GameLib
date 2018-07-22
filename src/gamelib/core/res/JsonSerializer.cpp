#include "gamelib/core/res/JsonSerializer.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/utils/json.hpp"
#include <fstream>

namespace gamelib
{
    bool JsonSerializer::loadFromFile(const std::string& fname, bool usefname)
    {
        if (usefname)
            _fname = fname;
        Json::Value node;
        if (loadJsonFromFile(fname, node))
            return loadFromJson(node);
        return false;
    }

    bool JsonSerializer::writeToFile(const std::string& fname, bool usefname)
    {
        if (usefname)
            _fname = fname;
        Json::Value node;
        writeToJson(node);
        return writeJsonToFile(fname, node);
    }

    bool JsonSerializer::reload()
    {
        if (!_fname.empty())
            return loadFromFile(_fname);
        LOG_ERROR("No filename given: must load from file before reloading");
        return false;
    }

    void JsonSerializer::write()
    {
        if (!_fname.empty())
            writeToFile(_fname);
        else
            LOG_ERROR("No filename given: must load from file before rewriting");
    }


    bool JsonSerializer::loadFromJson(const Json::Value& node)
    {
        LOG_ERROR("Not implemented");
        return false;
    }

    void JsonSerializer::writeToJson(Json::Value& node)
    {
        LOG_ERROR("Not implemented");
    }
}
