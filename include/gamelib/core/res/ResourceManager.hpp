#ifndef GAMELIB_RESOURCE_MANAGER_HPP
#define GAMELIB_RESOURCE_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <boost/filesystem.hpp>
#include "Resource.hpp"
#include "gamelib/core/Subsystem.hpp"
#include "gamelib/json/JsonSerializer.hpp"

// ResourceManager loads resources from given searchpaths.
//
// The user can add new searchpaths with addSearchpath().
//
// When a file is requested, ResourceManager will go through
// all searchpaths, from back to front, so that newer searchpaths
// will be searched first, and look for the file.
// If it does not exist in searchpath or the filename is absolute,
// it will search for it relative to cwd or absolute.
// If a absolute or relative file path turns out to lead to a searchpath,
// it will be treated as such correctly.
//
// writeToJson() will save all loaded filepaths. If the file was found
// in searchpath, it will be stored as relative path, otherwise absolute.
//
// If you want the cwd or executable location as searchpath, you need to
// specify it manually.
//
// It is possible to load shadowed files (a file that was softly overriden
// due to another file in a newer searchpath with the same name) by loading it
// relative to cwd or absolute.
//
// BUT: shadowed files cannot be correctly serialized in writeToJson() and
// appear as normal relative file paths, like other files found in searchpaths.
// 
// To prevent possible segfaults after calling clean(), objects should store
// the corresponding resource handle to keep up the reference count.

// Config file structure:
// (Lines starting with # are comments and are not valid json.)
//
// {
//     # The searchpath
//     "searchpath": [ "<searchpath>", ... ],
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


namespace gamelib
{
    class ResourceManager : public JsonSerializer, public Subsystem<ResourceManager>
    {
        public:
            typedef BaseResourceHandle(*LoaderCallback)(const std::string& fname, ResourceManager* resmgr);

            ASSIGN_NAMETAG("ResourceManager");

        public:
            ResourceManager();

            auto loadFromJson(const Json::Value& node) -> bool final override;
            auto writeToJson(Json::Value& node) const  -> void final override;

            // (Re-)Load a resource.
            auto load(const boost::filesystem::path& fname) -> BaseResourceHandle;

            // Same as load, but don't cache the resource
            auto loadOnce(const boost::filesystem::path& fname) -> BaseResourceHandle;

            // Free a resource (if it's not referenced anymore)
            auto free(const boost::filesystem::path& fname) -> void;

            // Return the resource and load it if it doesn't exist.
            auto get(const boost::filesystem::path& fname) -> BaseResourceHandle;

            // Same as get, but don't cache the resource
            auto getOnce(const boost::filesystem::path& fname) -> BaseResourceHandle;

            // Check if the resource exists and return a (null)pointer to it.
            auto find(const boost::filesystem::path& fname) -> BaseResourceHandle;

            // Returns the first resource of the given type (if any).
            // If the type is InvalidID the first resource of any type is returned.
            auto find(ID type) -> BaseResourceHandle;

            template <typename T>
            auto find() -> typename T::Handle
            {
                return find(T::id).template as<T>();
            }

            // Don't load anything, only search for the given file and
            // return its full path or empty path if it doesn't exist.
            auto findFile(const boost::filesystem::path& fname) const -> boost::filesystem::path;

            // Link a file extension to a loader-callback
            auto registerFileType(const std::string& ext, LoaderCallback cb) -> void;

            auto addSearchpath(const boost::filesystem::path& path)    -> bool;
            auto removeSearchpath(const boost::filesystem::path& path) -> bool;
            auto getSearchpaths() const -> const std::vector<boost::filesystem::path>&;

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
            auto _extractSearchpath(
                    const boost::filesystem::path& fullpath,
                    const boost::filesystem::path** searchpath,
                    boost::filesystem::path* relfname) const
                -> bool;

        private:
            std::unordered_map<std::string, BaseResourceHandle> _res;
            std::unordered_map<std::string, LoaderCallback> _typemap;
            std::vector<boost::filesystem::path> _searchpaths;
    };
}

#endif
