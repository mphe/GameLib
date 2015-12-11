#ifndef GAMELIB_CONVERSIONS_HPP
#define GAMELIB_CONVERSIONS_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "math/geometry/Vector2.hpp"
#include "math/geometry/AABB.hpp"

namespace conversions
{
    template <class T>
    geometry::Vector2<T> convert(const sf::Vector2<T>& vec)
    {
        return geometry::Vector2<T>(vec.x, vec.y);
    }

    template <class T>
    geometry::AABB<T> convert(const sf::Rect<T>& rect)
    {
        return geometry::AABB<T>(rect.left, rect.top, rect.width, rect.height);
    }

    template <class T>
    sf::Vector2<T> convert(const geometry::Vector2<T>& vec)
    {
        return sf::Vector2<T>(vec.x, vec.y);
    }

    template <class T>
    sf::Rect<T> convert(const geometry::AABB<T>& rect)
    {
        return sf::Rect<T>(rect.pos.x, rect.pos.y, rect.size.x, rect.size.y);
    }
}

#endif
