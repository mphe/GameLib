#ifndef GAMELIB_TILEMAP_COLLIDER_HPP
#define GAMELIB_TILEMAP_COLLIDER_HPP

#include "StaticTilemap.hpp"
#include "gamelib/collision/Collidable.hpp"

/*
 * TilemapCollider is a Collidable that checks for collisions with a tilemap
 * using a given callback.
 *
 * If the tiletype (T) is a Collidable, the Collidable-call will forwarded to
 * the object
 */

namespace gamelib
{
    /* Hides the actual implementation. Useful when a TilemapCollider of any
     * type is needed, for example in TileWalker.
     */
    class BaseTilemapCollider : public Collidable
    {
        public:
            virtual ~BaseTilemapCollider() {};
            virtual const int* getTileSize() const = 0;
    };

    template <class T>
    class TilemapCollider : public BaseTilemapCollider
    {
        typedef bool(*Callback)(const T&);

        public:
            TilemapCollider(const StaticTilemap<T>& map,
                    Callback callback = [](const T&){ return true; });

            bool contains(const math::Vec2f& point) const;
            bool intersects(const math::AABB<float>& rect) const;
            math::AABB<float> getBBox() const;

            const int* getTileSize() const;

        private:
            const StaticTilemap<T>* _map;
            Callback _callback;
    };

    namespace internal
    {
        template <class T, bool = std::is_base_of<Collidable, T>::value>
        struct CollidableForwarder
        {
            static bool contains(const T& t, const math::Vec2f& point)
            {
                return t.contains(point);
            }

            static bool intersects(const T& t, const math::AABB<float>& rect)
            {
                return t.intersects(rect);
            }
        };

        template <class T>
        struct CollidableForwarder<T, false>
        {
            constexpr static bool contains(const T& t, const math::Vec2f& point)
            {
                return true;
            }

            constexpr static bool intersects(const T& t, const math::AABB<float>& rect)
            {
                return true;
            }
        };
    }
}

#include "TilemapCollider.inl"

#endif
