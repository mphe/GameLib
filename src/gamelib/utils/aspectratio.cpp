#include "gamelib/utils/aspectratio.hpp"
#include "gamelib/utils/conversions.hpp"

namespace gamelib
{
    math::AABBf applyAspectRatio(const math::AABBf& rect, const math::AABBf& screen, AspectRatio ratio)
    {
        if (ratio == Fit)
        {
            auto ratio = screen.size / rect.size;
            float fac = std::min(ratio.x, ratio.y);
            auto size = rect.size * fac;
            auto pos = screen.pos + (screen.size - size) / 2;
            return math::AABBf(pos.asPoint(), size);
        }
        else if (ratio == Centered)
        {
            auto pos = screen.pos + (screen.size - rect.size) / 2;
            return math::AABBf(pos.asPoint(), rect.size);
        }
        else if (ratio == CenteredFit)
        {
            if (rect.size < screen.size)
                return applyAspectRatio(rect, screen, Centered);
            return applyAspectRatio(rect, screen, Fit);
        }
        else
            return screen;
    }

    sf::View applyAspectRatio(const sf::View& view, const sf::RenderTarget& target, AspectRatio ratio)
    {
        // get view rect
        auto& size = convert(view.getSize());
        auto pos = convert(view.getCenter()) - size / 2;
        math::AABBf rect(pos.asPoint(), size);

        // Create view rect from 2 points
        auto screensize = convert(target.getSize());
        auto& viewport = convert(view.getViewport());
        math::AABBf screen((viewport.pos * screensize).asPoint(), (viewport.size * screensize).asPoint());

        auto newrect = applyAspectRatio(rect, screen, ratio);
        newrect.pos /= screensize;
        newrect.size /= screensize;

        auto newview = view;
        newview.setViewport(convert(newrect));

        return newview;
    }
}
