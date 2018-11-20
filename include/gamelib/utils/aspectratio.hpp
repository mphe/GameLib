#ifndef GAMELIB_UTILS_ASPECT_RATIO_HPP
#define GAMELIB_UTILS_ASPECT_RATIO_HPP

#include "math/geometry/AABB.hpp"
#include <SFML/Graphics.hpp>

namespace gamelib
{
    enum AspectRatio
    {
        Stretch,
        Fit,
        Centered,
        CenteredFit,
        NumRatios
    };

    constexpr const char* str_aspectratios[] = { "Stretch", "Fit", "Centered", "Centered Fit" };

    math::AABBf applyAspectRatio(const math::AABBf& rect, const math::AABBf& screen, AspectRatio ratio);
    sf::View applyAspectRatio(const sf::View& view, const sf::RenderTarget& target, AspectRatio ratio);
}

#endif
