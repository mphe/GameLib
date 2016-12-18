#ifndef STATIC_RENDER_TILEMAP_HPP
#define STATIC_RENDER_TILEMAP_HPP

#include <vector>
#include <string>
#include "math/geometry/AABB.hpp"
#include "gamelib/sprite/SpriteSet.hpp"
#include "gamelib/Updatable.hpp"
#include "StaticDataSetTilemap.hpp"

namespace sf
{
    class RenderTarget;
}

namespace gamelib
{
    class StaticRenderTilemap : public StaticDataSetTilemap<SpriteData, SpriteSet>, public Updatable
    {
        public:
            StaticRenderTilemap(const SpriteSet& spriteset);
            ~StaticRenderTilemap() {}

            void update(float elapsed);
            void render(sf::RenderTarget& surface, const math::AABB<float>& rect);

        private:
            size_t _tilesRendered;
    };
}

#endif
