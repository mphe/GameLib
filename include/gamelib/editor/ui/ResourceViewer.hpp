#ifndef EDITOR_RESOURCE_VIEWER_HPP
#define EDITOR_RESOURCE_VIEWER_HPP

#include <vector>
#include "gamelib/core/res/Resource.hpp"
#include "gamelib/imgui/FileDialog.hpp"

namespace gamelib
{
    class ResourceViewer
    {
        public:
            ResourceViewer();
            ~ResourceViewer();

            auto open()  -> void;
            auto close() -> void;
            auto draw()  -> void;

        private:
            auto _refresh() -> void;

        private:
            struct CacheEntry
            {
                const char* name;
                ID id;
            };

        private:
            bool _open;
            int _category;
            std::vector<CacheEntry> _cache;
            BaseResourceHandle _selected;
            FileDialog _dialog;
    };
}

#endif
