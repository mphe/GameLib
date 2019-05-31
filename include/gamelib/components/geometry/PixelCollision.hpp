#ifndef GAMELIB_PIXEL_COLLISON_HPP
#define GAMELIB_PIXEL_COLLISON_HPP

#include <SFML/Graphics.hpp>
#include "gamelib/components/CollisionComponent.hpp"
#include "gamelib/core/res/TextureResource.hpp"

namespace gamelib
{
    class PixelCollision : public CollisionComponent
    {
        public:
            ASSIGN_NAMETAG("PixelCollision");

        public:
            PixelCollision();
            PixelCollision(float x, float y, float w, float h, unsigned int flags = 0);
            PixelCollision(const math::AABBf& aabb, unsigned int flags = 0);
            virtual ~PixelCollision() {}

            auto intersect(const math::Point2f& point) const -> bool final override;
            auto intersect(const math::Line2f& line) const   -> Intersection final override;
            auto intersect(const math::AABBf& rect) const    -> Intersection final override;

            auto sweep(const math::AABBf& rect, const math::Vec2f& vel) const -> Intersection final override;

            auto getNormal(float x, float y) const       -> math::Vec2f;
            auto getNormal(const math::Point2f& p) const -> math::Vec2f;

            auto getBBox() const     -> math::AABBf final override;

            auto loadFromJson(const Json::Value& node) -> bool final override;
            auto writeToJson(Json::Value& node) const  -> void final override;

            // Queries the ResourceManager or falls back to normal loading
            auto loadImageFromFile(const std::string& fname)       -> bool;
            auto loadImageFromTexture(const sf::Texture& tex)      -> void;
            auto loadImageFromTexture(TextureResource::Handle tex) -> void;

            auto getImage() const -> const sf::Image&;
            auto getImage()       -> sf::Image&;

            auto setMaskColor(sf::Color mask) -> void;

        protected:
            virtual auto _onChanged(const sf::Transform& old) -> void override;

        protected:
            sf::Color _mask;
            math::AABBf _rect;
            sf::Image _img;
            std::string _texname;
    };
}

#endif
