#include "gamelib/utils/aspectratio.hpp"
#include "gamelib/utils/conversions.hpp"

namespace gamelib
{
    math::AABBf applyAspectRatio(const math::Vec2f& camsize, const math::AABBf& screen, AspectRatio ratio)
    {
        if (ratio == Fit)
        {
            const auto r = (screen.size / camsize);
            const auto endsize = camsize * std::min(r.x, r.y);
            const auto pos = screen.pos + (screen.size - endsize) / 2;
            return math::AABBf(pos.asPoint(), endsize);
        }
        else if (ratio == Centered)
        {
            const auto pos = screen.pos + (screen.size - camsize) / 2;
            return math::AABBf(pos.asPoint(), camsize);
        }
        else if (ratio == CenteredFit)
        {
            if (camsize < screen.size)
                return applyAspectRatio(camsize, screen, Centered);
            return applyAspectRatio(camsize, screen, Fit);
        }
        else
            return screen;
    }

    math::AABBf applyAspectRatio(
            const math::Vec2f& camsize, const math::AABBf& viewport,
            const math::Vec2f& canvasSize, AspectRatio ratio)
    {
        // Calculate viewport rect in pixels
        auto screen = math::AABBf::fromPoints(
                (viewport.pos * canvasSize).asPoint(),
                (viewport.size * canvasSize).asPoint());

        // Apply aspect ratio
        auto newrect = applyAspectRatio(camsize, screen, ratio);

        // Convert back to 0-1 range
        newrect.pos /= canvasSize;
        newrect.size /= canvasSize;

        return newrect;
    }

    sf::View applyAspectRatio(const sf::View& view, const sf::RenderTarget& target, AspectRatio ratio)
    {
        const auto& camsize = convert(view.getSize());
        const auto screensize = convert(target.getSize());
        const auto& viewport = convert(view.getViewport());

        auto newview = view;
        newview.setViewport(convert(
                    applyAspectRatio(camsize, viewport, screensize, ratio)));

        return newview;
    }
}
