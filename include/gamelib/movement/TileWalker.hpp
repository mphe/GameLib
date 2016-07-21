#ifndef GAMELIB_TILE_WALKER_HPP
#define GAMELIB_TILE_WALKER_HPP

#include "Physical.hpp"
#include "gamelib/tile/TilemapCollider.hpp"
#include "gamelib/collision/AABBCollision.hpp"

/*
 * A Physical based on TilemapCollider.
 */

namespace gamelib
{
    class TileWalker : public Physical, public AABBCollision
    {
        public:
            TileWalker(float x, float y, float w, float h, const BaseTilemapCollider& c);
            virtual ~TileWalker() {};

            void move(float x, float y);

            // Splits up movement in x and y direction.
            // Moves first in x direction, then y direction
            void moveContact(float x, float y);

            geometry::Vector2<float> getPosition() const;

        private:
            void _moveContact(float val, int index);

        private:
            const BaseTilemapCollider* _col;
    };
}

#endif
