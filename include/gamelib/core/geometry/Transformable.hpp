#ifndef GAMELIB_TRANSFORMABLE_HPP
#define GAMELIB_TRANSFORMABLE_HPP

#include "gamelib/utils/utils.hpp"
#include "math/geometry/Vector.hpp"
#include "math/geometry/AABB.hpp"
#include <SFML/Graphics/Transform.hpp>

namespace gamelib
{
    class GroupTransform;

    constexpr float zeroscale = 0.0001;

    class TransformData
    {
        public:
            TransformData();

            auto getMatrix() const -> sf::Transform;
            auto reset()           -> void;

            auto operator-=(const TransformData& rhs) -> TransformData&;
            auto operator+=(const TransformData& rhs) -> TransformData&;

        public:
            math::Point2f origin;
            math::Point2f pos;
            math::Vec2f scale;
            float angle;
    };


    class Transformable
    {
        friend class GroupTransform;

        public:
            Transformable();
            Transformable(bool movable, bool scalable, bool rotatable);
            virtual ~Transformable();

            // Diable move/copy
            Transformable(const Transformable&) = delete;
            Transformable(Transformable&&)      = delete;
            auto operator=(const Transformable&) -> Transformable& = delete;
            auto operator=(Transformable&&)      -> Transformable& = delete;

            virtual auto getBBox() const -> math::AABBf = 0;

            auto getLocalBBox() const -> math::AABBf;

            auto setLocalPosition(const math::Point2f& pos)  -> void;
            auto setLocalScale(const math::Vec2f& scale)     -> void;
            auto setLocalRotation(float angle)               -> void;
            auto setLocalTransformation(const TransformData& data) -> void;

            auto getLocalPosition() const -> const math::Point2f&;
            auto getLocalScale() const    -> const math::Vec2f&;
            auto getLocalRotation() const -> float;
            auto getLocalTransformation() const -> const TransformData&;
            auto getLocalMatrix() const -> sf::Transform;

            auto setPosition(const math::Point2f& pos)  -> void;
            auto setPosition(float x, float y)          -> void;
            auto setScale(const math::Vec2f& scale)     -> void;
            auto setScale(float x, float y)             -> void;
            auto setRotation(float angle)               -> void;
            auto setTransformation(const TransformData& data)      -> void;

            auto getPosition() const -> const math::Point2f&;
            auto getScale() const    -> const math::Vec2f&;
            auto getRotation() const -> float;
            auto getTransformation() const -> TransformData;
            auto getMatrix() const         -> const sf::Transform&;

            auto move(const math::Vec2f& rel)           -> void;
            auto move(float x, float y)                 -> void;
            auto scale(const math::Vec2f& scale)        -> void;
            auto scale(float x, float y)                -> void;
            auto rotate(float angle)                    -> void;

            auto setOrigin(const math::Point2f& origin) -> void;
            auto setOrigin(float x, float y)            -> void;
            auto getOrigin() const   -> const math::Point2f&;

            auto reset() -> void;

            auto getParent() const   -> GroupTransform*;

            auto isMovable() const   -> bool;
            auto isScalable() const  -> bool;
            auto isRotatable() const -> bool;

            auto operator-=(const TransformData& rhs) -> Transformable&;
            auto operator+=(const TransformData& rhs) -> Transformable&;

        protected:
            virtual auto _onChanged(UNUSED const sf::Transform& old) -> void {};

            // Tell parent to update its bounding box
            auto _markDirty() const -> void;
            auto _setSupportedOps(bool movable, bool scalable, bool rotatable) -> void;

        private:
            // Called by GroupTransform on adding
            auto _setParent(GroupTransform* parent) -> void;

            // Recomputes the matrix. Also called by parents when they changed
            auto _updateMatrix() -> void;

        private:
            bool _movable, _scalable, _rotatable;
            GroupTransform* _parent;
            TransformData _local;
            struct _GlobalData {
                math::Point2f pos;
                math::Vec2f scale;
                float angle;

                _GlobalData() : scale(1, 1), angle(0) {}
            } _global;
            sf::Transform _matrix;
    };
}

#endif
