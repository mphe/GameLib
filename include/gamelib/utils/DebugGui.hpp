#ifndef GAMELIB_DEBUG_GUI_HPP
#define GAMELIB_DEBUG_GUI_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "gamelib/core/rendering/Renderable.hpp"
#include "string.hpp"

namespace gamelib
{
    class DebugGui : public Renderable
    {
        public:
            DebugGui(const sf::Font& font, unsigned int size = 14);
            virtual ~DebugGui() {};

            void setColor(const sf::Color& col);

            template <class... Args>
            void drawText(float x, float y, Args&&... args)
            {
                _text.emplace_back(join(std::forward<Args>(args)...), *_font, _fontsize);
                // _text.back().setStyle(sf::Text::Bold);
                _text.back().setPosition(x, y);
                _text.back().setColor(_color);
            }

            template <class... Args>
            void drawDebug(Args&&... args)
            {
                drawText(8, 8 + _dbgSize++ * _fontsize, std::forward<Args>(args)...);
            }

            void render(sf::RenderTarget& target) final override;

        private:
            template <class T, class... Args>
            void _joinString(std::stringstream& ss, const T& arg, Args&&... rest)
            {
                ss << arg;
                _joinString(ss, std::forward<Args>(rest)...);
            }

            template <class T>
            void _joinString(std::stringstream& ss, const T& arg)
            {
                ss << arg;
            }

        private:
            int _fontsize;
            int _dbgSize;
            sf::Color _color;
            const sf::Font* _font;
            std::vector<sf::Text> _text;
    };
}

#endif
