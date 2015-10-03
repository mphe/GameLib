#ifndef FLOATING_TILE_HPP
#define FLOATING_TILE_HPP

#include "Tile.hpp"
#include <SFML/Graphics.hpp>
#include "math/geometry/Vector2.hpp"

namespace engine
{
    class TileSet;

    class FloatingTile : public Tile
    {
        public:
            FloatingTile(const TileSet& tileset, const Json::Value& params);
            FloatingTile(const TileSet& tileset, TileID id, float x, float y);
            virtual ~FloatingTile() {};

            virtual bool loadFromJson(const Json::Value& params);
            virtual void update(float fps);
            virtual void render(sf::RenderTarget& target) const;

        protected:
            void _change(TileID id);
            sf::IntRect _getTexRect() const;

        protected:
            const TileSet& _tileset;
            sf::Sprite _sprite;
    };
}

#endif
