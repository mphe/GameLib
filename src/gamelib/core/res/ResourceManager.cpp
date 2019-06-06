#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/events/ResourceReloadEvent.hpp"
#include "gamelib/core/event/EventManager.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/utils/string.hpp"
#include <boost/filesystem.hpp>

namespace gamelib
{
    ResourceManager::ResourceManager(const std::string& searchpath)
    {
        setSearchpath(searchpath);
    }

    bool ResourceManager::loadFromJson(const Json::Value& node)
    {
        if (node.isMember("searchpath"))
            setSearchpath(node["searchpath"].asString());

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

                std::string subfolder = it.key().asString();
                if (!subfolder.empty())
                    assureDelimiter(&subfolder);

                if (reload)
                {
                    clean();
                    for (auto& fname : *it)
                        load(subfolder + fname.asString());
                }
                else
                {
                    for (auto& fname : *it)
                        res.push_back(get(subfolder + fname.asString()));
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

                std::string subfolder = it.key().asString();
                if (!subfolder.empty())
                    assureDelimiter(&subfolder);

                if (reload)
                    for (auto& fname : *it)
                        loadOnce(subfolder + fname.asString());
                else
                    for (auto& fname : *it)
                        getOnce(subfolder + fname.asString());
            }
        }

        return true;
    }

    void ResourceManager::writeToJson(Json::Value& node) const
    {
        node["searchpath"] = _searchpath;
        node["forcereload"] = false;

        // TODO: group files by subfolders
        auto& files = node["preload"][""];
        files.resize(_res.size());

        Json::ArrayIndex i = 0;
        for (auto& it : _res)
            files[i++] = boost::filesystem::relative(it.first, _searchpath).string();
    }

    BaseResourceHandle ResourceManager::load(const std::string& fname)
    {
        auto res = loadOnce(fname);
        if (!res)
            return nullptr;

        auto path = res.getResource()->getPath();

        // Fire a reload event if the resource was reloaded
        auto evmgr = getSubsystem<EventManager>();
        if (evmgr)
        {
            auto oldres = find(path);
            if (oldres && oldres.use_count() > 1)
                evmgr->queueEvent(ResourceReloadEvent::create(path, res));
        }

        _res[path] = res;
        return res;
    }

    BaseResourceHandle ResourceManager::loadOnce(const std::string& fname)
    {
        LOG_DEBUG("Loading file ", fname, "...");

        auto pos = fname.find_last_of('.');
        if (pos == std::string::npos)
        {
            LOG_ERROR("File has no extension: ", fname);
            return nullptr;
        }

        auto ext = fname.substr(pos + 1);
        auto it = _typemap.find(ext);
        if (it == _typemap.end())
        {
            LOG_ERROR("Unknown resource type: ", fname);
            return nullptr;
        }

        std::string path = _preparePath(fname);

        // Call the associated loader
        auto res = it->second(path, this);
        if (!res)
        {
            LOG_ERROR("Failed to load resource ", path);
            return nullptr;
        }

        // res.getResource()->_path = path.data() + _searchpath.size();
        res.getResource()->_path = path;
        return res;
    }

    void ResourceManager::free(const std::string& fname)
    {
        auto it = _res.find(_preparePath(fname));
        if (it != _res.end())
            if (it->second.use_count() == 1)
            {
                LOG_DEBUG_WARN("Freeing resource ", it->first);
                _res.erase(it);
            }
    }

    BaseResourceHandle ResourceManager::get(const std::string& fname)
    {
        auto ptr = find(fname);
        if (ptr)
            return ptr;
        return load(fname);
    }

    BaseResourceHandle ResourceManager::getOnce(const std::string& fname)
    {
        auto ptr = find(fname);
        if (ptr)
            return ptr;
        return loadOnce(fname);
    }

    BaseResourceHandle ResourceManager::find(const std::string& fname)
    {
        auto it = _res.find(_preparePath(fname));
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

    void ResourceManager::setSearchpath(const std::string& path)
    {
        _searchpath = path;
        adaptPath(&_searchpath);
        assureDelimiter(&_searchpath);
    }

    const std::string& ResourceManager::getSearchpath() const
    {
        return _searchpath;
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
        _searchpath.clear();
        LOG_DEBUG_WARN("ResourceManager destroyed");
    }

    std::string ResourceManager::_preparePath(const std::string& fname) const
    {
        std::string path;
        if (boost::filesystem::path(fname).is_absolute())
            path = _searchpath + boost::filesystem::relative(fname, _searchpath).string();
        else
            path = _searchpath + fname;

        path = boost::filesystem::path(path).lexically_normal().string();
        adaptPath(&path);

        if (!boost::filesystem::exists(path))
            LOG_WARN("File does not exist locally: ", path);

        return path;
    }
}
