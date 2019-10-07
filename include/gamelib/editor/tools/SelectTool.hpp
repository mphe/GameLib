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

            void onMousePressed() final override;
            void onMouseRelease() final override;
            void onDrag() final override;

            void render(sf::RenderTarget& target) final override;
            void drawGui() final override;

            auto select(EntityReference ent)      -> EntityReference;
            auto selectComponent(Component* comp) -> Component*;
            auto select(float x, float y)         -> EntityReference;
            auto find(float x, float y)           -> EntityReference;

            auto getSelected() const          -> EntityReference;
            auto getSelectedComponent() const -> Component*;

        public:
            bool renderBBox;
            bool renderAllBoxes;

        private:
            EntityReference _selected;
            Component* _selectedcomp;
            math::Vec2f _dragoffset;
            size_t _scaleselect;
            int _mode;
            bool _cloned;
            bool _switchmode;
            bool _dragged;
    };
}

#endif
