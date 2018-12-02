#ifndef GAMELIB_TRANSFORMABLEAABB_HPP
#define GAMELIB_TRANSFORMABLEAABB_HPP

#include "Transformable.hpp"

namespace gamelib
{
    class TransformableAABB : public Transformable
    {
        public:
            TransformableAABB();
            TransformableAABB(const math::AABBf& rect);
            TransformableAABB(float x, float y, float w, float h);
            virtual ~TransformableAABB() {}

            virtual auto setSize(const math::Vec2f& size) -> void;
            auto setSize(float w, float h)        -> void;
            auto getSize() const                  -> const math::Vec2f&;

            virtual auto getBBox() const -> const math::AABBf&;

        protected:
            virtual auto _onChanged(const sf::Transform& old) -> void;

        private:
            math::Vec2f _size;
            math::AABBf _rect;
    };
}

#endif
