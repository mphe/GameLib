#ifndef GAMELIB_TILEMAP_ADAPTER_HPP
#define GAMELIB_TILEMAP_ADAPTER_HPP

#include "../GameObject.hpp"
#include "../tile/StaticRenderTilemap.hpp"

/*
 * A simple wrapper around StaticRenderTilemap that provides a Renderable
 * interface.
 */

namespace gamelib
{
    class Scene;

    class TilemapAdapter : public GameObject
    {
        public:
            TilemapAdapter(const Scene& scene, const SpriteSet& set);
            void update(float fps);
            void render(sf::RenderTarget& target);

            StaticRenderTilemap& operator*();
            const StaticRenderTilemap& operator*() const;
            StaticRenderTilemap* operator->();
            const StaticRenderTilemap* operator->() const;

        public:
            StaticRenderTilemap map;

        private:
            const Scene& scene;
    };
}

#endif
