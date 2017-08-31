#ifndef GAMELIB_TILE_WALKER_HPP
#define GAMELIB_TILE_WALKER_HPP

#include "Physical.hpp"
#include "gamelib/geometry/TilemapCollider.hpp"
#include "gamelib/geometry/AABBCollision.hpp"

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

            // Splits up movement in x and y direction.
            // Moves first in x direction, then y direction
            void moveContact(float x, float y);

            bool placeFree(float x, float y) const;
            bool placeFreeRelative(float x, float y) const;

        private:
            void _moveContact(float val, int index);

        private:
            const BaseTilemapCollider* _col;
    };
}

#endif
