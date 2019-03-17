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
            SpriteTool();

            void onMousePressed() final override;
            void drawGui() final override;
            void render(sf::RenderTarget& target) final override;

        private:
            SpriteResource::Handle _sprite;
    };
}

#endif
