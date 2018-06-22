#ifndef EDITOR_LAYER_UI_HPP
#define EDITOR_LAYER_UI_HPP

#include "gamelib/core/rendering/Layer.hpp"

namespace gamelib
{
    bool drawLayerUI(bool* open = nullptr);
    bool inputLayer(const std::string& label, Layer::Handle* handle);
}

#endif
