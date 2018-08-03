#ifndef GAMELIB_GROUP_TRANSFORM_HPP
#define GAMELIB_GROUP_TRANSFORM_HPP

#include <vector>
#include "Transformable.hpp"

namespace gamelib
{
    class GroupTransform : public Transformable
    {
        friend class Transformable;

        public:
            using Transformable::move;
            using Transformable::scale;
            using Transformable::rotate;

        public:
            GroupTransform();
            GroupTransform(float x, float y);
            GroupTransform(const math::Point2f& pos);
            virtual ~GroupTransform() {};

            auto add(Transformable* trans, bool relative = true)    -> void;
            auto remove(Transformable* trans, bool relative = true) -> void;

            auto move(const math::Vec2f& rel)    -> void;
            auto scale(const math::Vec2f& scale) -> void;
            auto rotate(float angle)             -> void;

            auto getPosition() const -> const math::Point2f&;
            auto getScale() const    -> const math::Vec2f&;
            auto getRotation() const -> float;
            auto getBBox() const     -> const math::AABBf&;

            auto getChildren() const -> const std::vector<Transformable*>&;

        protected:
            mutable math::AABBf _bbox;
            math::Point2f _pos;
            math::Vec2f _scale;
            float _rotation;
            std::vector<Transformable*> _objs;
            mutable bool _dirty; // set by children, marks if bounding box needs to be recalculated
    };
}

#endif
