#ifndef GAMELIB_TRANSFORMABLE_HPP
#define GAMELIB_TRANSFORMABLE_HPP

#include "math/geometry/Vector.hpp"
#include "math/geometry/AABB.hpp"

namespace gamelib
{
    class Transformable
    {
        public:
            constexpr static unsigned int movable = 1;
            constexpr static unsigned int rotatable = 1 << 1;
            constexpr static unsigned int scalable = 1 << 2;

        public:
            Transformable(unsigned int supported = 0);
            virtual ~Transformable() {};

            auto getSupportedOps() const -> unsigned int;

            virtual auto move(const math::Vec2f& rel)    -> void;
            virtual auto scale(const math::Vec2f& scale) -> void;
            virtual auto rotate(float angle)             -> void;

            virtual auto getPosition() const -> const math::Point2f&;
            virtual auto getScale() const    -> const math::Vec2f&;
            virtual auto getRotation() const -> float;

            virtual auto getBBox() const     -> const math::AABBf&;

            auto setPosition(const math::Point2f& pos) -> void;
            auto setPosition(float x, float y)         -> void;
            auto move(float x, float y)                -> void;
            auto setScale(const math::Vec2f& scale)    -> void;
            auto setScale(float x, float y)            -> void;
            auto scale(float x, float y)               -> void;
            auto setRotation(float angle)              -> void;

            auto reset() -> void;

            auto operator-=(const Transformable& rhs) -> Transformable&;
            auto operator+=(const Transformable& rhs) -> Transformable&;

        protected:
            unsigned int _supported;
    };
}

#endif
