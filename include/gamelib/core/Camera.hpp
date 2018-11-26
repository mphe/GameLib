#ifndef GAMELIB_CAMERA_HPP
#define GAMELIB_CAMERA_HPP

#include "math/geometry/AABB.hpp"
#include "gamelib/core/IDCounter.hpp"
#include "gamelib/core/update/Updatable.hpp"
#include "gamelib/utils/aspectratio.hpp"
#include <SFML/Graphics.hpp>

namespace gamelib
{
    class Camera : public Updatable
    {
        public:
            Camera(const std::string& name);
            Camera(const std::string& name, const math::Vec2f& pos, const math::Vec2f& size);

            bool loadFromJson(const Json::Value& node);
            void writeToJson(Json::Value& node);

            void update(float elapsed);

            void setMotion(float speed, float dir);
            void addMotion(float speed, float dir);

            void center(float x, float y);
            void center(const math::Vec2f& pos);
            void move(float x, float y);

            void zoomTowards(float x, float y, float zoom);

            auto getCamRect() const -> math::AABB<float>;
            auto getName() const -> const std::string&;

            sf::View getView() const;
            sf::View getView(const sf::RenderTarget& target) const;

            void apply(sf::RenderTarget& target) const;

        public:
            float zoom;
            math::Vec2f pos;
            math::Vec2f size;
            math::AABBf viewport;
            math::Vec2f vel;
            AspectRatio ratio;

        private:
            std::string _name;
    };
}

#endif
