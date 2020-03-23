#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/events/ResourceReloadEvent.hpp"
#include "gamelib/core/event/EventManager.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/utils/string.hpp"
#include "gamelib/json/json-resources.hpp"
#include <boost/filesystem.hpp>

namespace gamelib
{
    // Requires both paths to be canonical
    auto isUnderDirectory(const boost::filesystem::path& fname, const boost::filesystem::path& dir, boost::filesystem::path* relfname = nullptr) -> bool
    {
        boost::filesystem::path rel;
        if (!relfname)
            relfname = &rel;

        *relfname = fname.lexically_relative(dir);
        return !relfname->empty() && !relfname->begin()->filename_is_dot_dot();
    }

    ResourceManager::ResourceManager()
    {
    }

    bool ResourceManager::loadFromJson(const Json::Value& node)
    {
        if (node.isMember("searchpath"))
        {
            const auto& searchpathnode = node["searchpath"];
            if (!searchpathnode.isArray())
                LOG_ERROR("Wrong searchpath format, should be array");
            else
                for (Json::ArrayIndex i = 0; i < searchpathnode.size(); ++i)
                    addSearchpath(searchpathnode[i].asString());
        }

        bool reload = node.get("forcereload", false).asBool();

        if (!node.isMember("preload"))
            clean();
        else
        {
            // Store references to prevent cleanup of loaded resources
            // Only used when forcereload is false
            std::vector<BaseResourceHandle> res;

            auto& preload = node["preload"];
            for (Json::Value::const_iterator it = preload.begin(); it != preload.end(); ++it)
            {
                if (it->empty())
                    continue;

                boost::filesystem::path subfolder = it.key().asString();

                if (reload)
                {
                    clean();
                    for (auto& fname : *it)
                        load(subfolder / fname.asString());
                }
                else
                {
                    for (auto& fname : *it)
                        res.push_back(get(subfolder / fname.asString()));
                    clean();
                }
            }
        }

        if (node.isMember("once"))
        {
            auto& once = node["once"];
            for (Json::Value::const_iterator it = once.begin(); it != once.end(); ++it)
            {
                if (it->empty())
                    continue;

                boost::filesystem::path subfolder = it.key().asString();
                for (auto& fname : *it)
                    if (reload)
                        loadOnce(subfolder / fname.asString());
                    else
                        getOnce(subfolder / fname.asString());
            }
        }

        return true;
    }

    void ResourceManager::writeToJson(Json::Value& node) const
    {
        auto& searchpathnode = node["searchpath"];
        for (auto& i : _searchpaths)
        {
            boost::filesystem::path rel;
            if (isUnderDirectory(i, boost::filesystem::current_path(), &rel))
                searchpathnode.append(rel.string());
            else
                searchpathnode.append(i.string());
        }

        node["forcereload"] = false;

        // TODO: group files by subfolders
        auto& files = node["preload"][""];
        files.resize(_res.size());

        Json::ArrayIndex i = 0;
        for (auto& it : _res)
            ::gamelib::writeToJson(files[i++], it.second, this);
    }

    BaseResourceHandle ResourceManager::load(const boost::filesystem::path& fname)
    {
        auto res = loadOnce(fname);
        if (!res)
            return nullptr;

        auto path = findFile(res.getResource()->getFullPath());
        auto pathstring = path.string();

        // Fire a reload event if the resource was reloaded
        auto oldres = find(path);
        if (oldres && oldres.use_count() > 1)
            queueEvent<ResourceReloadEvent>(pathstring, res);

        _res[pathstring] = res;
        return res;
    }

    BaseResourceHandle ResourceManager::loadOnce(const boost::filesystem::path& fname)
    {
        LOG("Loading file ", fname, "...");

        const auto ext = fname.extension().string().substr(1);

        if (ext.empty())
        {
            LOG_ERROR("File has no extension");
            return nullptr;
        }

        auto it = _typemap.find(ext);
        if (it == _typemap.end())
        {
            LOG_ERROR("Unknown resource type: ", ext);
            return nullptr;
        }

        auto loadpath = findFile(fname);
        if (loadpath.empty())
            return nullptr;

        // Call the associated loader
        // TODO: make_preferred() ?
        auto res = it->second(loadpath.string(), this);
        if (!res)
        {
            LOG_ERROR("Failed to load resource ", loadpath);
            return nullptr;
        }

        // Set resource path attributes
        BaseResource& meta = *res.getResource();
        const boost::filesystem::path *searchpath = nullptr;
        boost::filesystem::path relpath;

        if (_extractSearchpath(loadpath, &searchpath, &relpath))
        {
            meta._searchpath = searchpath->string();
            meta._path = relpath.string();
        }
        else
            meta._path = loadpath.string();

        return res;
    }

    void ResourceManager::free(const boost::filesystem::path& fname)
    {
        auto it = _res.find(findFile(fname).string());
        if (it != _res.end() && it->second.use_count() == 1)
        {
            LOG_DEBUG_WARN("Freeing resource ", it->first);
            _res.erase(it);
        }
    }

    BaseResourceHandle ResourceManager::get(const boost::filesystem::path& fname)
    {
        auto ptr = find(fname);
        if (ptr)
            return ptr;
        return load(fname);
    }

    BaseResourceHandle ResourceManager::getOnce(const boost::filesystem::path& fname)
    {
        auto ptr = find(fname);
        if (ptr)
            return ptr;
        return loadOnce(fname);
    }

    BaseResourceHandle ResourceManager::find(const boost::filesystem::path& fname)
    {
        auto it = _res.find(findFile(fname).string());
        if (it == _res.end())
            return nullptr;
        return it->second;
    }

    BaseResourceHandle ResourceManager::find(ID type)
    {
        BaseResourceHandle res;
        foreach([&](const std::string&, BaseResourceHandle res_) {
                res = res_;
                return true;
            }, type);
        return res;
    }

    void ResourceManager::registerFileType(const std::string& ext, LoaderCallback cb)
    {
        _typemap[ext] = cb;
        LOG_DEBUG("Registered filetype ", ext);
    }

    auto ResourceManager::addSearchpath(const boost::filesystem::path& path) -> bool
    {
        if (boost::filesystem::exists(path))
        {
            _searchpaths.push_back(boost::filesystem::canonical(path));
            return true;
        }
        LOG_ERROR("Searchpath does not exist: ", path);
        return false;
    }

    auto ResourceManager::removeSearchpath(const boost::filesystem::path& path) -> bool
    {
        if (boost::filesystem::exists(path))
        {
            auto it = std::find(_searchpaths.begin(), _searchpaths.end(), boost::filesystem::canonical(path));
            if (it != _searchpaths.end())
                _searchpaths.erase(it);
            return true;
        }
        LOG_ERROR("Searchpath does not exist: ", path);
        return false;
    }

    auto ResourceManager::getSearchpaths() const -> const std::vector<boost::filesystem::path>&
    {
        return _searchpaths;
    }

    void ResourceManager::clean()
    {
        LOG_DEBUG_WARN("Freeing unreferenced resources");

        // The do-loop ensures that all resources are freed correctly when
        // there are resources that reference others, e.g.
        // SpriteResource -> TextureResource.
        // This does _not_ solve cyclic dependencies!
        int freed;
        do
        {
            freed = 0;
            for (auto it = _res.begin(); it != _res.end();)
            {
                if (it->second.use_count() == 1)
                {
                    LOG_DEBUG_WARN("\t", it->first);
                    it = _res.erase(it);
                    ++freed;
                }
                else
                    ++it;
            }
        } while (freed > 0);
    }

    void ResourceManager::clear()
    {
        _res.clear();
        LOG_DEBUG_WARN("Freeing all resources");
    }

    void ResourceManager::destroy()
    {
        _res.clear();
        _typemap.clear();
        _searchpaths.clear();
        LOG_DEBUG_WARN("ResourceManager destroyed");
    }

    auto ResourceManager::findFile(const boost::filesystem::path& fname) const -> boost::filesystem::path
    {
        // TODO: maybe allow files not to exist

        if (fname.is_relative())
            for (auto it = _searchpaths.rbegin(), end = _searchpaths.rend(); it != end; ++it)
                if (boost::filesystem::exists(*it / fname))
                    return boost::filesystem::canonical(fname, *it);

        if (boost::filesystem::exists(fname))
            return boost::filesystem::canonical(fname);

        LOG_ERROR("File does not exist: ", fname);
        return boost::filesystem::path();
    }

    auto ResourceManager::_extractSearchpath(
            const boost::filesystem::path& fullpath,
            const boost::filesystem::path** searchpath,
            boost::filesystem::path* relfname) const
        -> bool
    {
        assert(searchpath && "Must not be null");
        assert(relfname && "Must not be null");

        for (auto it = _searchpaths.rbegin(), end = _searchpaths.rend(); it != end; ++it)
        {
            boost::filesystem::path rel;
            if (isUnderDirectory(fullpath, *it, &rel))
            {
                *searchpath = &*it;
                *relfname = std::move(rel);
                return true;
            }
        }

        LOG_WARN("File does not exist in any searchpath: ", fullpath);
        return false;
    }
}
