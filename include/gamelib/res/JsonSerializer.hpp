#ifndef GAMELIB_JSON_SERIALIZER_HPP
#define GAMELIB_JSON_SERIALIZER_HPP

#include <string>
#include "json/json.h"

namespace gamelib
{
    class JsonSerializer
    {
        public:
            virtual ~JsonSerializer() {}
            virtual bool loadFromFile(const std::string& fname);
            virtual bool writeToFile(const std::string& fname);
            virtual bool reload();

            // Will be called by loadFromFile(), after the file was read.
            // Derived classes should implement this.
            virtual bool loadFromJson(const Json::Value& node);

            // Same as above but for writeToFile().
            virtual void writeToJson(Json::Value& node);

        protected:
            std::string _fname;
    };
}

#endif
