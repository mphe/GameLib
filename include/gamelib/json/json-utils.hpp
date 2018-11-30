#ifndef GAMELIB_JSON_UTILS_HPP
#define GAMELIB_JSON_UTILS_HPP

#include "json/json.h"

namespace gamelib
{
    // Compares node against compare and writes everything non-existent or different in compare to out.
    bool diffJson(const Json::Value& node, const Json::Value& compare, Json::Value* out_);

    // Recursively merge the src dict into dest
    void mergeJson(const Json::Value& src, Json::Value* dest);
}

#endif
