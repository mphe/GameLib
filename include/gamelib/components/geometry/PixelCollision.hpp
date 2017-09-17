#ifndef GAMELIB_PIXEL_COLLISON_HPP
#define GAMELIB_PIXEL_COLLISON_HPP

#include <SFML/Graphics.hpp>
#include "gamelib/core/ecs/CollisionComponent.hpp"

namespace gamelib
{
    class PixelCollision : public CollisionComponent
    {
        public:
            constexpr static const char* name = "PixelCollision";

        public:
            PixelCollision();
            PixelCollision(float x, float y, float w, float h, unsigned int flags = 0);
            PixelCollision(const math::AABBf& aabb, unsigned int flags = 0);
            virtual ~PixelCollision() {}

            auto intersect(const math::Point2f& point) const -> bool;
            auto intersect(const math::Line2f& line) const   -> Intersection;
            auto intersect(const math::AABBf& rect) const    -> Intersection;

            auto sweep(const math::AABBf& rect, const math::Vec2f& vel) const -> Intersection;

            auto getNormal(float x, float y) const       -> math::Vec2f;
            auto getNormal(const math::Point2f& p) const -> math::Vec2f;

            auto move(const math::Vec2f& rel) -> void;
            auto getPosition() const          -> const math::Point2f&;
            auto getBBox() const              -> const math::AABBf&;

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

            // Queries the ResourceManager or falls back to normal loading
            auto loadImageFromFile(const std::string& fname) -> bool;
            auto loadImageFromTexture(const sf::Texture& tex) -> void;
            auto getImage() const -> const sf::Image&;

        protected:
            math::AABBf _rect;
            sf::Image _img;
            std::string _texname;
    };
}

#endif
