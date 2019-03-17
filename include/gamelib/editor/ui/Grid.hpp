#ifndef EDITOR_GRID_HPP
#define EDITOR_GRID_HPP

#include <SFML/Graphics.hpp>
#include "math/geometry/Point2.hpp"
#include "gamelib/core/rendering/Renderable.hpp"

namespace gamelib
{
    class Grid : public Renderable
    {
        public:
            Grid(int w, int h);

            auto increase() -> void;
            auto decrease() -> void;

            auto snap(const math::Point2f& p) const -> math::Point2f;

            auto render(sf::RenderTarget& target) -> void final override;

        private:
            auto _recalculate(const sf::Vector2f& size) -> void;

        private:
            math::Vec2i _gridsize;
            sf::Vector2f _lastsize;
            sf::VertexArray _cache;
    };
}

#endif
