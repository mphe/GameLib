#ifndef GAMELIB_DEBUG_GUI_HPP
#define GAMELIB_DEBUG_GUI_HPP

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

namespace gamelib
{
    class DebugGui
    {
        public:
            DebugGui(const sf::Font& font, unsigned int size = 16);
            virtual ~DebugGui() {};

            void setColor(const sf::Color& col);

            void drawText(float x, float y, const std::string& text);
            void drawDebug(const std::string& text);

            void render(sf::RenderTarget& target);

        private:
            int _fontsize;
            int _dbgSize;
            sf::Color _color;
            const sf::Font& _font;
            std::vector<sf::Text> _text;
    };
}

#endif
