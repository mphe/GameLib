#include "utils/JsonObject.hpp"
#include "utils/log.hpp"
#include <fstream>

namespace Json
{
    bool JsonObject::loadFromJson(const std::string& fname)
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

    bool JsonObject::reload()
    {
        return loadFromJson(_fname);
    }
}
