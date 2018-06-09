#ifndef GAMELIB_SPRITE_RESOURCE_HPP
#define GAMELIB_SPRITE_RESOURCE_HPP

#include "Resource.hpp"
#include "TextureResource.hpp"
#include "gamelib/core/sprite/SpriteData.hpp"
#include "math/geometry/AABB.hpp"

/*
 * Config file structure:
 * {
 *     "texture": "texture",
 *     "framepos": [ x, y ],
 *     "framesize": [ w, h ],
 *     "length": length,
 *     "speed": speed,
 *     "offset": offset
 * }
 */

namespace gamelib
{
    class ResourceManager;

    void registerSpriteLoader(ResourceManager& resmgr);

    BaseResourceHandle spriteLoader(const std::string& fname, ResourceManager* resmgr);

    struct SpriteResourceData
    {
        TextureResource::Handle tex;
        AnimationData ani;
        math::AABBi rect;
    };

    typedef Resource<SpriteResourceData, 0xd5b8e3bf> SpriteResource;
}

#endif
