#include "gamelib/editor/ui/ResourceViewer.hpp"
#include "gamelib/imgui/resources.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "imgui.h"
#include <boost/filesystem.hpp>

namespace gamelib
{
    ResourceViewer::ResourceViewer() :
        _open(false),
        _category(0),
        _dialog(FileDialog::Load)
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

        if (_dialog.process())
            if (getSubsystem<ResourceManager>()->load(_dialog.getPath()))
                _refresh();

        if (ImGui::Begin("Resource Manager", &_open))
        {
            if (ImGui::Button("Load"))
                _dialog.open(getSubsystem<ResourceManager>()->getSearchpath().c_str());

            ImGui::SameLine();
            ImGui::Spacing();
            ImGui::SameLine();

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

        resmgr->foreach([&](const std::string& fname, BaseResourceHandle res) {
                auto id = res.getResource()->getID();
                if (std::find_if(_cache.begin(), _cache.end(), [id](const CacheEntry& entry) {
                            return entry.id == id;
                        }) == _cache.end())
                {
                    _cache.emplace_back();
                    _cache.back().id = id;
                    getResourceMeta(id, &_cache.back().name, nullptr, nullptr);
                }
                return false;
        });
    }
}
