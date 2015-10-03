#ifndef UTILS_JSON_OBJECT_HPP
#define UTILS_JSON_OBJECT_HPP

#include <string>
#include "json/json.h"

namespace Json
{
    class JsonObject
    {
        public:
            virtual ~JsonObject() {}
            virtual bool loadFromJson(const std::string& fname);
            virtual bool loadFromJson(const Json::Value& node) { return true; }
            virtual bool reload();

        protected:
            std::string _fname;

    };
}

#endif
