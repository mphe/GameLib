#ifndef GAMELIB_RESOURCE_MANAGER_HPP
#define GAMELIB_RESOURCE_MANAGER_HPP

#include <string>
#include <unordered_map>
#include "Resource.hpp"
#include "gamelib/core/event/EventManager.hpp"
#include "gamelib/core/Subsystem.hpp"
#include "JsonSerializer.hpp"

// To prevent possible segfaults after calling clean(), objects should store
// the corresponding resource handle to keep up the reference count.

// Config file structure:
// (Lines starting with # are comments and are not valid json.)
//
// {
//     # The searchpath
//     "searchpath": "<searchpath>",
//
//     # Forces reloading of already loaded resources instead of reusing them
//     # Default is false.
//     "forcereload": <true/false>,
//
//     # Files to load
//     "preload": {
//         # files in the searchpath directory
//         "": [
//             "dira/dirb/file1",
//             "file2",
//             ...
//         ],
//         # files in the subfolder "directory"
//         "<directory>": [
//             "file1",
//             "dirc/file2",
//             ...
//         ],
//         ...
//     }
// }


// TODO: Iterators

namespace gamelib
{
    class ResourceManager : public JsonSerializer, public Subsystem<ResourceManager>
    {
        public:
            typedef BaseResourceHandle(*LoaderCallback)(const std::string&, ResourceManager* resmgr);

            constexpr static const char* name = "ResourceManager";

        public:
            ResourceManager() {};
            ResourceManager(const std::string& searchpath);

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

            // (Re-)Load a resource.
            auto load(const std::string& fname) -> BaseResourceHandle;

            // Same as load, but don't cache the resource
            auto loadOnce(const std::string& fname) -> BaseResourceHandle;

            // Free a resource (if it's not referenced anymore)
            auto free(const std::string& fname) -> void;

            // Return the resource and load it if it doesn't exist.
            auto get(const std::string& fname) -> BaseResourceHandle;

            // Same as get, but don't cache the resource
            auto getOnce(const std::string& fname) -> BaseResourceHandle;

            // Check if the resource exists and return a (null)pointer to it.
            auto find(const std::string& fname) -> BaseResourceHandle;

            // Link a file extension to a loader-callback
            auto registerFileType(const std::string& ext, LoaderCallback cb) -> void;

            auto setSearchpath(const std::string& path) -> void;

            // Free all resources that aren't referenced anywhere else
            auto clean() -> void;

            // Free all resources
            auto clear() -> void;

            // Free all resources, clear searchpath and remove loader callbacks
            auto destroy() -> void;

            // Calls a callback for each resource (of the given type).
            // Callback format: callback(const std::string&, BaseResourceHandle)
            template <typename F>
            auto foreach(F callback, ID type = invalidID) -> void
            {
                for (auto it = _res.begin(); it != _res.end(); ++it)
                    if (type == invalidID || it->second.getResource()->getID() == type)
                        callback(it->first, it->second);
            }

        private:
            std::unordered_map<std::string, BaseResourceHandle> _res;
            std::unordered_map<std::string, LoaderCallback> _typemap;
            std::string _searchpath;
    };
}

#endif
