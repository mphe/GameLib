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

            auto onMousePressed()                 -> void;
            auto drawGui()                        -> void;
            auto render(sf::RenderTarget& target) -> void;
            auto refresh()                        -> void;

        private:
            int _selected;
            std::vector<std::string> _entities;
    };
}

#endif
