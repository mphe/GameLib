#include "gamelib/json/json-utils.hpp"
#include "gamelib/utils/log.hpp"
#include "math/type_traits.hpp"
#include <cassert>

namespace gamelib
{
    bool diffJson(const Json::Value& node, const Json::Value& compare, Json::Value* out_)
    {
        assert(out_ && "Destination node is null");
        auto& out = *out_;

        if (node.isNumeric() && compare.isNumeric())
        {
            if (node.isIntegral() && compare.isIntegral() && node.asInt() == compare.asInt())
                return false;
            else if (math::almostEquals(node.asDouble(), compare.asDouble(), 0.00001))
                return false;
            else if (math::almostEquals(node.asDouble(), compare.asDouble(), 0.0001))
                LOG_DEBUG_WARN("A double value differs with tolerance 0.0001 but not ", 0.00001);
        }
        else if (node.type() == compare.type())
        {
            if (node.isObject())
            {
                bool diff = false;
                for (auto it = node.begin(), end = node.end(); it != end; ++it)
                {
                    auto key = it.key().asString();
                    if (!compare.isMember(key))
                    {
                        out[key] = *it;
                        diff = true;
                    }
                    else
                    {
                        Json::Value val;
                        if (diffJson(*it, compare[key], &val))
                        {
                            out[key] = val;
                            diff = true;
                        }
                    }
                }
                return diff;
            }
            else if (node.isArray())
            {
                if (node.size() == compare.size())
                {
                    for (Json::ArrayIndex i = 0; i < node.size(); ++i)
                    {
                        Json::Value tmp;
                        if (diffJson(node[i], compare[i], &tmp))
                        {
                            out = node;
                            return true;
                        }
                    }
                    return false;
                }
            }
            else if (node.compare(compare) == 0)
                return false;
        }

        out = node;
        return true;
    }

    void mergeJson(const Json::Value& src, Json::Value* dest)
    {
        assert(dest && "Destination node is null");

        for (auto it = src.begin(), end = src.end(); it != end; ++it)
        {
            auto& out = (*dest)[it.key().asString()];

            if (it->isObject())
                mergeJson(*it, &out);
            else
                out = *it;
        }
    }
}
