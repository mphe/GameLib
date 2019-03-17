#ifndef GAMELIB_CAMERA_HPP
#define GAMELIB_CAMERA_HPP

#include "math/geometry/AABB.hpp"
#include "gamelib/core/update/Updatable.hpp"
#include "gamelib/json/JsonSerializer.hpp"
#include "gamelib/utils/aspectratio.hpp"
#include <SFML/Graphics.hpp>

namespace gamelib
{
    class Camera : public Updatable, public JsonSerializer
    {
        public:
            Camera(const std::string& name);
            Camera(const std::string& name, const math::Point2f& pos, const math::Vec2f& size);
            virtual ~Camera() {}

            virtual bool loadFromJson(const Json::Value& node) override;
            virtual void writeToJson(Json::Value& node) const override;

            virtual void update(float elapsed) override;

            void setMotion(float speed, float dir);
            void addMotion(float speed, float dir);

            void center(float x, float y);
            void center(const math::Point2f& pos);
            void move(float x, float y);

            void zoomTowards(float x, float y, float zoom);

            auto getCamRect() const -> math::AABB<float>;
            auto getName() const -> const std::string&;

            sf::View getView() const;
            sf::View getView(const sf::RenderTarget& target) const;

            void apply(sf::RenderTarget& target) const;

        public:
            float zoom;
            math::Point2f pos;
            math::Vec2f size;
            math::AABBf viewport;
            math::Vec2f vel;
            AspectRatio ratio;
            bool active;

        private:
            std::string _name;
    };
}

#endif
