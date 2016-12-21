#ifndef GAMELIB_CONVERSIONS_HPP
#define GAMELIB_CONVERSIONS_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "math/geometry/Vector.hpp"
#include "math/geometry/AABB.hpp"

namespace conversions
{
    template <class T>
    math::Vec2<T> convert(const sf::Vector2<T>& vec)
    {
        return math::Vec2<T>(vec.x, vec.y);
    }

    template <class T>
    math::AABB<T> convert(const sf::Rect<T>& rect)
    {
        return math::AABB<T>(rect.left, rect.top, rect.width, rect.height);
    }

    template <class T>
    sf::Vector2<T> convert(const math::Vec2<T>& vec)
    {
        return sf::Vector2<T>(vec.x, vec.y);
    }

    template <class T>
    sf::Rect<T> convert(const math::AABB<T>& rect)
    {
        return sf::Rect<T>(rect.pos.x, rect.pos.y, rect.size.x, rect.size.y);
    }
}

#endif
