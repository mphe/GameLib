#ifndef EDITOR_SPRITE_TOOL_HPP
#define EDITOR_SPRITE_TOOL_HPP

#include <vector>
#include <string>
#include "Tool.hpp"
#include "gamelib/core/res/SpriteResource.hpp"

namespace gamelib
{
    class SpriteTool : public Tool
    {
        public:
            auto onMousePressed()                 -> void;
            auto drawGui()                        -> void;
            auto render(sf::RenderTarget& target) -> void;

        private:
            SpriteResource::Handle _sprite;
    };
}

#endif
