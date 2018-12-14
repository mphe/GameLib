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
            virtual bool loadFromFile(const std::string& fname, bool usefname = true);
            virtual bool writeToFile(const std::string& fname, bool usefname = true) const;

            virtual bool reload();
            virtual void write() const;

            // Will be called by loadFromFile(), after the file was read.
            // Derived classes should implement this.
            virtual bool loadFromJson(const Json::Value& node) = 0;

            // Same as above but for writeToFile().
            virtual void writeToJson(Json::Value& node) const = 0;

        protected:
            mutable std::string _fname;
    };
}

#endif
