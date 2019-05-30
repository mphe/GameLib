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
            GroupTransform();
            virtual ~GroupTransform() {};

            auto add(Transformable* trans)    -> void;
            auto remove(Transformable* trans) -> void;

            auto getChildren() const -> const std::vector<Transformable*>&;

            virtual auto getBBox() const -> math::AABBf override;

        protected:
            virtual auto _onChanged(const sf::Transform& old) -> void override;

        protected:
            std::vector<Transformable*> _objs;
            mutable math::AABBf _bbox;
            mutable bool _dirty; // set by children, marks if bounding box needs to be recalculated
    };
}

#endif
