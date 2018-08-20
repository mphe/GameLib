#ifndef EDITOR_SELECTTOOL_HPP
#define EDITOR_SELECTTOOL_HPP

#include "Tool.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/geometry/flags.hpp"

namespace gamelib
{
    class SelectTool : public Tool
    {
        static constexpr int mode_move = 0;
        static constexpr int mode_scale = 1;
        static constexpr int mode_rotate = 2;
        static constexpr int num_modes = 2;

        public:
            SelectTool();

            auto onMousePressed() -> void;
            auto onMouseRelease() -> void;
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
            size_t _scaleselect;
            int _mode;
            bool _cloned;
            bool _switchmode;
    };
}

#endif
