#ifndef EDITOR_VERTEXTOOL_HPP
#define EDITOR_VERTEXTOOL_HPP

#include "Tool.hpp"

namespace gamelib
{
    class VertexTool : public Tool
    {
        public:
            VertexTool();

            void onMousePressed();
            void onDrag();
            void drawGui();
            void render(sf::RenderTarget& target);

        private:
            int _selected;
            bool _snappoint;
    };
}

#endif
