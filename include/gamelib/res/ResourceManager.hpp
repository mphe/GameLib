#ifndef GAMELIB_RESOURCE_MANAGER_HPP
#define GAMELIB_RESOURCE_MANAGER_HPP

#include <string>
#include <unordered_map>
#include "Resource.hpp"
#include "gamelib/event/EventManager.hpp"
#include "JsonSerializer.hpp"

// To prevent possible segfaults after calling clean(), objects should store
// the corresponding ResourcePtr to keep up the reference count.

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
//     "preload": [
//         "file1",
//         "file2",
//         ...
//     ]
// }


// TODO: Iterators

namespace gamelib
{
    class ResourceManager : public JsonSerializer
    {
        public:
            typedef ResourcePtr(*LoaderCallback)(const std::string&, ResourceManager* resmgr);

        public:
            ResourceManager(EventManager* evmgr = nullptr);
            ResourceManager(EventManager* evmgr, const std::string& searchpath);

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

            // (Re-)Load a resource.
            auto load(const std::string& fname) -> ResourcePtr;

            // Return the resource and load it if it doesn't exist.
            auto get(const std::string& fname) -> ResourcePtr;

            // Check if the resource exists and return a (null)pointer to it.
            auto find(const std::string& fname) -> ResourcePtr;

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
            // Callback format: callback(const std::string&, BaseResource&)
            template <typename F>
            auto foreach(F callback, ID type = invalidID) -> void
            {
                for (auto it = _res.begin(); it != _res.end(); ++it)
                    if (type == invalidID || it->second->getID() == type)
                        callback(it->first, (*it->second));
            }

        private:
            EventManager* _evmgr;
            std::unordered_map<std::string, ResourcePtr> _res;
            std::unordered_map<std::string, LoaderCallback> _typemap;
            std::string _searchpath;
    };

    class ResourceReloadEvent : public Event<0x05eb2119, ResourceReloadEvent>
    {
        public:
            ResourceReloadEvent(const std::string& fname, ResourcePtr ptr);

            // The new resource's filename (relative to search path)
            std::string fname;

            // The new resource
            ResourcePtr res;
    };
}

#endif
