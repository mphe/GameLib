#ifndef GAMELIB_TILEMAP_COLLIDER_HPP
#define GAMELIB_TILEMAP_COLLIDER_HPP

#include <functional>
#include "gamelib/tile/StaticTilemap.hpp"
#include "Collidable.hpp"

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
        public:
            TilemapCollider(const StaticTilemap<T>& map);

            template <typename F>
            TilemapCollider(const StaticTilemap<T>& map, F callback);

            template <typename F>
            auto setCallback(F callback) -> void;

            auto intersect(const math::Point2f& point) const -> bool;
            auto intersect(const math::Line2f& line) const   -> Intersection;
            auto intersect(const math::AABBf& rect) const -> Intersection;

            auto move(float x, float y)         -> void;
            auto setPosition(float x, float y)  -> void;

            auto getPosition() const    -> const math::Vec2f&;
            auto getBBox() const        -> const math::AABBf;

            const int* getTileSize() const;

        private:
            const StaticTilemap<T>* _map;
            math::Vec2f _pos;
            std::function<bool(const T&)> _callback;
    };

    // TODO: Maybe remove this, it's ugly
    namespace internal
    {
        template <class T, bool = std::is_base_of<Collidable, T>::value>
        struct CollidableForwarder
        {
            static bool contains(const T& t, const math::Point2f& point)
            {
                return t.intersect(point);
            }

            static bool intersects(const T& t, const math::AABBf& rect)
            {
                return t.intersect(rect);
            }
        };

        template <class T>
        struct CollidableForwarder<T, false>
        {
            constexpr static bool contains(const T& t, const math::Point2f& point)
            {
                return true;
            }

            constexpr static bool intersects(const T& t, const math::AABBf& rect)
            {
                return true;
            }
        };
    }
}

#include "TilemapCollider.inl"

#endif
