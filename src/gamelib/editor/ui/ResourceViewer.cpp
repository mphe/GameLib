#include "gamelib/editor/ui/ResourceViewer.hpp"
#include "gamelib/editor/ui/resources.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "imgui.h"

namespace gamelib
{
    ResourceViewer::ResourceViewer() :
        _open(false),
        _category(0)
    { }

    ResourceViewer::~ResourceViewer()
    {
        close();
    }

    void ResourceViewer::open()
    {
        if (!_open)
        {
            _open = true;
            _refresh();
        }
    }

    void ResourceViewer::close()
    {
        if (_open)
        {
            _open = false;
            _cache.clear();
            _selected.reset();
        }
    }

    void ResourceViewer::draw()
    {
        if (!_open)
            return;

        if (ImGui::Begin("Resource Viewer", &_open))
        {
            if (ImGui::Button("Refresh"))
                _refresh();

            ImGui::SameLine();
            ImGui::Spacing();
            ImGui::SameLine();

            if (ImGui::Combo("Category", &_category, [](void* data, int index, const char** name) -> bool {
                        const auto& cache = *static_cast<decltype(_cache)*>(data);
                        if (index < 0 || index >= cache.size())
                            return false;
                        *name = cache[index].name;
                        return true;
                    }, &_cache, _cache.size()))
            _selected.reset();

            ImGui::NewLine();
            drawResourceSelection(&_selected, _cache[_category].id, true);
        }
        ImGui::End();

        if (!_open)
            close();
    }


    void ResourceViewer::_refresh()
    {
        if (!_open)
            return;

        _selected.reset();
        _cache.clear();
        _category = 0;

        auto resmgr = getSubsystem<ResourceManager>();
        if (!resmgr)
            return;

        resmgr->foreach ([&](const std::string& fname, BaseResourceHandle res) {
                auto id = res.getResource()->getID();
                if (std::find_if(_cache.begin(), _cache.end(), [id](const CacheEntry& entry) {
                            return entry.id == id;
                        }) == _cache.end())
                {
                    _cache.emplace_back();
                    _cache.back().id = id;
                    getResourceMeta(id, &_cache.back().name, nullptr, nullptr);
                }
        });
    }
}
