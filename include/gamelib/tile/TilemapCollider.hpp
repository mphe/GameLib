#ifndef GAMELIB_TILEMAP_COLLIDER_HPP
#define GAMELIB_TILEMAP_COLLIDER_HPP

#include "StaticTilemap.hpp"
#include "gamelib/collision/Collidable.hpp"
#include "gamelib/Identifier.hpp"

/*
 * TilemapCollider is a Collidable that checks for collisions with a tilemap
 * using a given callback.
 *
 * If the tiletype (T) is a Collidable, the Collidable-call will forwarded to
 * the object
 */

namespace gamelib
{
    template <class T>
    class TilemapCollider : public Collidable
    {
        typedef bool(*Callback)(const T&);

        public:
            TilemapCollider(const StaticTilemap<T>& map,
                    Callback callback = [](const T&){ return true; });

            bool contains(const geometry::Vector2<float>& point) const;
            bool intersects(const geometry::AABB<float>& rect) const;
            geometry::AABB<float> getBBox() const;

        private:
            const StaticTilemap<T>& _map;
            Callback _callback;
    };

    namespace internal
    {
        template <class T, bool = std::is_base_of<Collidable, T>::value>
        struct CollidableForwarder
        {
            static bool contains(const T& t, const geometry::Vector2<float>& point)
            {
                return t.contains(point);
            }

            static bool intersects(const T& t, const geometry::AABB<float>& rect)
            {
                return t.intersects(rect);
            }
        };

        template <class T>
        struct CollidableForwarder<T, false>
        {
            constexpr static bool contains(const T& t, const geometry::Vector2<float>& point)
            {
                return true;
            }

            constexpr static bool intersects(const T& t, const geometry::AABB<float>& rect)
            {
                return true;
            }
        };
    }
}

#include "TilemapCollider.inl"

#endif
