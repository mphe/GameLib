#ifndef EDITOR_RESOURCE_MANAGER_HPP
#define EDITOR_RESOURCE_MANAGER_HPP

#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/res/TextureResource.hpp"

namespace gamelib
{
    auto drawTextureSelect(TextureResource::Handle* tex) -> bool;
    auto drawTextureSelectPopup() -> TextureResource::Handle;
}

#endif
