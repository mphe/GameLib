#ifndef GAMELIB_CAMERA_HPP
#define GAMELIB_CAMERA_HPP

#include "gamelib/core/update/Updatable.hpp"
#include "gamelib/core/geometry/Transformable.hpp"
#include "gamelib/utils/aspectratio.hpp"
#include <SFML/Graphics.hpp>
#include <string>

namespace gamelib
{
    class Camera : public Updatable, public Transformable
    {
        public:
            Camera();
            Camera(const std::string& name);
            Camera(const math::Point2f& pos, const math::Vec2f& size);
            Camera(const std::string& name, const math::Point2f& pos, const math::Vec2f& size);

            auto getSize() const -> const math::Vec2f&;
            auto setSize(const math::Vec2f& size) -> void;

            auto zoom(float zoom)                                -> void;
            auto setZoom(float zoom)                             -> void;
            auto getZoom() const                                 -> float;
            auto zoomTowards(const math::Point2f& p, float zoom) -> void;
            auto zoomTowards(float x, float y, float zoom)       -> void;

            auto getBBox() const              -> math::AABBf final;
            auto update(UNUSED float elapsed) -> void override {};

            auto getView() const                               -> sf::View;
            auto getView(const sf::RenderTarget& target) const -> sf::View;

            // Shortcut for target.setView(getView(target))
            auto apply(sf::RenderTarget& target) const -> void;

        public:
            math::AABBf viewport;
            AspectRatio ratio;
            bool active;
            std::string name;

        protected:
            math::Vec2f _size;
    };
}

#endif
