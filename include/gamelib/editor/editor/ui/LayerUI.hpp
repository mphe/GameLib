#ifndef EDITOR_LAYER_UI_HPP
#define EDITOR_LAYER_UI_HPP

#include <vector>
#include <string>
#include "gamelib/core/rendering/Layer.hpp"

namespace gamelib
{
    class LayerUI
    {
        public:
            typedef Layer::Handle LayerHandle;

        public:
            LayerUI();

            auto drawLayerUI() -> void;
            auto drawSelector(const std::string& label, LayerHandle default_) const -> LayerHandle;

            auto refresh()          -> void;
            auto getCurrent() const -> LayerHandle;

        private:
            static bool _itemGetter(void* me_, int index, const char** name);
            void _add(LayerHandle handle = LayerHandle());

        private:
            struct LayerCache
            {
                LayerHandle handle;
                std::string name;
            };

        private:
            int _current;
            std::vector<LayerCache> _layers;
    };
}

#endif
