#include "gamelib/json/JsonSerializer.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/json/json-file.hpp"
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

    bool JsonSerializer::writeToFile(const std::string& fname, bool usefname) const
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

    void JsonSerializer::write() const
    {
        if (!_fname.empty())
            writeToFile(_fname);
        else
            LOG_ERROR("No filename given: must load from file before rewriting");
    }
}
