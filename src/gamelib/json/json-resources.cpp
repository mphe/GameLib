#include "gamelib/json/json-resources.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/utils/utils.hpp"
#include <boost/filesystem.hpp>

namespace gamelib
{
    void writeToJson(Json::Value& node, const BaseResourceHandle& handle, UNUSED const ResourceManager* resmgr)
    {
        if (handle)
            node = handle.getResource()->getPath();
        else
            node = "";
    }

    bool loadFromJson(const Json::Value& node, BaseResourceHandle* handle, ResourceManager* resmgr)
    {
        if (!node.isString())
        {
            LOG_ERROR("Wrong resource config format");
            return false;
        }

        handle->reset();

        auto fname = node.asString();
        if (!fname.empty())
        {
            resmgr = resmgr ? resmgr : getSubsystem<ResourceManager>();
            if (resmgr)
                *handle = resmgr->get(fname);
        }

        return true;
    }
}
