#ifndef EDITOR_ENTITYTOOL_HPP
#define EDITOR_ENTITYTOOL_HPP

#include <vector>
#include <string>
#include "Tool.hpp"

namespace gamelib
{
    class EntityTool : public Tool
    {
        public:
            EntityTool();

            void onMousePressed() final override;
            void drawGui() final override;
            void render(sf::RenderTarget& target) final override;
            void refresh();

        private:
            int _selected;
            std::vector<std::string> _entities;
    };
}

#endif
