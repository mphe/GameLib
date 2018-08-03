#ifndef GAMELIB_SCENE_OBJECT_HPP
#define GAMELIB_SCENE_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include "gamelib/core/geometry/Transformable.hpp"
#include "SceneData.hpp"
#include "Layer.hpp"

namespace gamelib
{
    class SceneObject : public SceneData,
                        public Transformable
    {
        public:
            SceneObject(int depth = 0, float parallax = 1, unsigned int flags = 0);
            virtual ~SceneObject() {}

            auto setLayer(Layer::Handle layer) -> void;
            auto getLayer() const              -> Layer::Handle;

            // Returns the flags merged with the layer's and scene's flags
            auto getAllFlags() const -> unsigned int;
            auto isVisible() const   -> bool;

            auto unregister() -> void;

            auto render(sf::RenderTarget& target) const -> void;
            virtual auto render(sf::RenderTarget& target, const sf::RenderStates& states) const -> void;

            virtual auto loadFromJson(const Json::Value& node) -> bool;
            virtual auto writeToJson(Json::Value& node)        -> void;

            // TODO: should go into Transformable
            auto setOrigin(const math::Point2f& origin) -> void;
            auto getOrigin() const                      -> const math::Point2f&;

            virtual auto move(const math::Vec2f& rel)    -> void;
            virtual auto scale(const math::Vec2f& scale) -> void;
            virtual auto rotate(float angle)             -> void;

            virtual auto getPosition() const -> const math::Point2f&;
            virtual auto getScale() const    -> const math::Vec2f&;
            virtual auto getRotation() const -> float;
            virtual auto getBBox() const     -> const math::AABBf&;

            auto getLocalBounds() const -> math::AABBf;
            auto getTransform() const -> sf::Transform;

        protected:
            // Should be called by parents whenever the vertex list changes
            auto _updateBBox() -> void;

        protected:
            sf::VertexArray _vertices;

        private:
            Layer::Handle _layer;   // TODO: Could be turned into a parent when Scene supports it
            math::AABBf _bbox;
            math::Point2f _pos;
            math::Vec2f _scale;
            math::Point2f _origin;
            float _rotation;
    };
}


#endif
