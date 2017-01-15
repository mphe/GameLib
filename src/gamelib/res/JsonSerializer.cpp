#include "gamelib/res/JsonSerializer.hpp"
#include "gamelib/utils/log.hpp"
#include <fstream>

namespace gamelib
{
    bool JsonSerializer::loadFromFile(const std::string& fname)
    {
        LOG("(Re)Loading ", fname, "...");
        _fname = fname;
        std::ifstream f;
        f.open(fname.c_str());
        if (f.is_open())
        {
            Json::Value doc;
            f >> doc;
            f.close();
            return loadFromJson(doc);
        }
        LOG_ERROR("Failed to open ", fname);
        return false;
    }

    bool JsonSerializer::writeToFile(const std::string& fname)
    {
        LOG("Writing to file ", fname, "...");

        // TODO: Think about if this would be a good idea.
        // Problems arise when a class implements loadFromJson(), but not
        // writeToJson(). A reload() would then try to load an empty file.
        // _fname = fname;

        std::ofstream f;
        f.open(fname.c_str());
        if (f.is_open())
        {
            Json::Value doc;
            writeToJson(doc);
            f << doc.toStyledString();
            f.close();
            return true;
        }
        LOG_ERROR("Failed to open ", fname);
        return false;
    }

    bool JsonSerializer::reload()
    {
        return loadFromFile(_fname);
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
