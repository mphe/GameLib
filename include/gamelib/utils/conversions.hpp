#ifndef GAMELIB_CONVERSIONS_HPP
#define GAMELIB_CONVERSIONS_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "math/geometry/Vector.hpp"
#include "math/geometry/AABB.hpp"

namespace gamelib
{
    template <class T>
    math::Vec2<T>& convert(sf::Vector2<T>& vec)
    {
        return *reinterpret_cast<math::Vec2<T>*>(&vec);
    }

    template <class T>
    const math::Vec2<T>& convert(const sf::Vector2<T>& vec)
    {
        return *reinterpret_cast<const math::Vec2<T>*>(&vec);
    }

    template <class T>
    sf::Vector2<T>& convert(math::Vec2<T>& vec)
    {
        return *reinterpret_cast<sf::Vector2<T>*>(&vec);
    }

    template <class T>
    const sf::Vector2<T>& convert(const math::Vec2<T>& vec)
    {
        return *reinterpret_cast<const sf::Vector2<T>*>(&vec);
    }

    template <class T>
    sf::Vector2<T>& convert(math::Point2<T>& vec)
    {
        return convert(vec.asVector());
    }

    template <class T>
    const sf::Vector2<T>& convert(const math::Point2<T>& vec)
    {
        return convert(vec.asVector());
    }


    template <class T>
    math::AABB<T>& convert(sf::Rect<T>& rect)
    {
        return *reinterpret_cast<math::AABB<T>*>(&rect);
    }

    template <class T>
    const math::AABB<T>& convert(const sf::Rect<T>& rect)
    {
        return *reinterpret_cast<const math::AABB<T>*>(&rect);
    }

    template <class T>
    sf::Rect<T>& convert(math::AABB<T>& rect)
    {
        return *reinterpret_cast<sf::Rect<T>*>(&rect);
    }

    template <class T>
    const sf::Rect<T>& convert(const math::AABB<T>& rect)
    {
        return *reinterpret_cast<const sf::Rect<T>*>(&rect);
    }

    // template <class T>
    // math::Vec2<T> convert(const sf::Vector2<T>& vec)
    // {
    //     return math::Vec2<T>(vec.x, vec.y);
    // }
    //
    // template <class T>
    // math::AABB<T> convert(const sf::Rect<T>& rect)
    // {
    //     return math::AABB<T>(rect.left, rect.top, rect.width, rect.height);
    // }
    //
    // template <class T>
    // sf::Vector2<T> convert(const math::Vec2<T>& vec)
    // {
    //     return sf::Vector2<T>(vec.x, vec.y);
    // }
    //
    // template <class T>
    // sf::Vector2<T> convert(const math::Point2<T>& p)
    // {
    //     return convert(p.asVector());
    // }
    //
    // template <class T>
    // sf::Rect<T> convert(const math::AABB<T>& rect)
    // {
    //     return sf::Rect<T>(rect.pos.x, rect.pos.y, rect.size.x, rect.size.y);
    // }


    template <typename T>
    sf::Vector2<T> operator*(const sf::Vector2<T>& a, const math::Vec2<T>& b)
    {
        return interpret(interpret(a) * b);
    }

    template <typename T>
    sf::Vector2<T> operator/(const sf::Vector2<T>& a, const math::Vec2<T>& b)
    {
        return interpret(interpret(a) / b);
    }

    template <typename T>
    sf::Vector2<T> operator*(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
    {
        return a * interpret(b);
    }

    template <typename T>
    sf::Vector2<T> operator/(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
    {
        return a / interpret(b);
    }
}

#endif
