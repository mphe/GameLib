#ifndef EDITOR_SELECTTOOL_HPP
#define EDITOR_SELECTTOOL_HPP

#include "Tool.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/geometry/flags.hpp"

namespace gamelib
{
    class SelectTool : public Tool
    {
        public:
            SelectTool();

            auto onMousePressed() -> void;
            auto onDrag()         -> void;

            auto render(sf::RenderTarget& target) -> void;
            auto drawGui()                        -> void;

            auto select(Entity::Handle enthandle)        -> void;
            auto select(Entity* ent)                     -> void;
            auto select(float x, float y, unsigned int flags = collision_mask) -> void;

            auto getSelected() const -> const Entity*;
            auto getSelected()       -> Entity*;

        public:
            bool renderBBox;
            bool renderAllBoxes;

        private:
            Entity::Handle _selected;
            math::Vec2f _dragoffset;
    };
}

#endif
