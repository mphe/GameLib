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

    // Return a rect fitted to screen rect with the given camera size and ratio mode.
    math::AABBf applyAspectRatio(
            const math::Vec2f& camsize, const math::AABBf& screen, AspectRatio ratio);

    // Calculates the correct view dimensions and returns the viewport coordinates
    math::AABBf applyAspectRatio(
            const math::Vec2f& camsize, const math::AABBf& viewport,
            const math::Vec2f& canvasSize, AspectRatio ratio);

    // Calculates and returns the aspect-ratio applied view for the given RenderTarget
    sf::View applyAspectRatio(
            const sf::View& view, const sf::RenderTarget& target, AspectRatio ratio);
}

#endif
