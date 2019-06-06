#ifndef GAMELIB_RESOURCE_MANAGER_HPP
#define GAMELIB_RESOURCE_MANAGER_HPP

#include <string>
#include <unordered_map>
#include "Resource.hpp"
#include "gamelib/core/Subsystem.hpp"
#include "gamelib/json/JsonSerializer.hpp"

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
//
//     # Files to load but without caching them.
//     # Useful for config files that are only applied once
//     "once": {
//         # same structure as "preload"
//     }
// }


// TODO: Iterators

namespace gamelib
{
    class ResourceManager : public JsonSerializer, public Subsystem<ResourceManager>
    {
        public:
            typedef BaseResourceHandle(*LoaderCallback)(const std::string& fname, ResourceManager* resmgr);

            ASSIGN_NAMETAG("ResourceManager");

        public:
            ResourceManager() {};
            ResourceManager(const std::string& searchpath);

            auto loadFromJson(const Json::Value& node) -> bool final override;
            auto writeToJson(Json::Value& node) const  -> void final override;

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

            // Returns the first resource of the given type (if any).
            // If the type is InvalidID the first resource of any type is returned.
            auto find(ID type) -> BaseResourceHandle;

            template <typename T>
            auto find() -> typename T::Handle
            {
                return find(T::id).template as<T>();
            }

            // Link a file extension to a loader-callback
            auto registerFileType(const std::string& ext, LoaderCallback cb) -> void;

            auto setSearchpath(const std::string& path) -> void;
            auto getSearchpath() const                  -> const std::string&;

            // Free all resources that aren't referenced anywhere else
            auto clean() -> void;

            // Free all resources
            auto clear() -> void;

            // Free all resources, clear searchpath and remove loader callbacks
            auto destroy() -> void;

            // Calls a callback for each resource (of the given type).
            // Loop breaks when the callback returns true.
            // Callback signature: bool(const std::string&, BaseResourceHandle)
            template <typename F>
            auto foreach(F callback, ID type = invalidID) -> void
            {
                for (auto it = _res.begin(); it != _res.end(); ++it)
                    if (type == invalidID || it->second.getResource()->getID() == type)
                        if (callback(it->first, it->second))
                            return;
            }

        private:
            auto _preparePath(const std::string& fname) const -> std::string;

        private:
            std::unordered_map<std::string, BaseResourceHandle> _res;
            std::unordered_map<std::string, LoaderCallback> _typemap;
            std::string _searchpath;
    };
}

#endif
