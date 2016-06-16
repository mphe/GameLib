#ifndef GAMELIB_TILEMAP_ADAPTER_HPP
#define GAMELIB_TILEMAP_ADAPTER_HPP

#include "../Renderable.hpp"
#include "../tile/StaticRenderTilemap.hpp"

/*
 * A simple wrapper around StaticRenderTilemap that provides a Renderable
 * interface.
 */

namespace gamelib
{
    class Scene;

    class TilemapAdapter : public Renderable
    {
        public:
            TilemapAdapter(const Scene& scene, const SpriteSet& set);
            void render(sf::RenderTarget& target);

        public:
            StaticRenderTilemap map;

        private:
            const Scene& scene;
    };
}

#endif
