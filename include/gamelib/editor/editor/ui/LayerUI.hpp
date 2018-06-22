#ifndef EDITOR_LAYER_UI_HPP
#define EDITOR_LAYER_UI_HPP

#include <vector>
#include <string>
#include "gamelib/core/rendering/Layer.hpp"

namespace gamelib
{
    void drawLayerUI();
    bool inputLayer(const std::string& label, Layer::Handle* handle);
}

#endif
