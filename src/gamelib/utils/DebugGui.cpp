#include "gamelib/utils/DebugGui.hpp"

namespace gamelib
{
    DebugGui::DebugGui(const sf::Font& font, unsigned int size) :
        _fontsize(size),
        _dbgSize(0),
        _color(sf::Color::Black),
        _font(font)
    { }


    void DebugGui::setColor(const sf::Color& col)
    {
        _color = col;
    }


    void DebugGui::drawText(float x, float y, const std::string& text)
    {
        sf::Text t(text, _font, _fontsize);
        t.setStyle(sf::Text::Bold);
        t.setPosition(x, y);
        t.setColor(_color);
        _text.push_back(t);
    }

    void DebugGui::drawDebug(const std::string& text)
    {
        drawText(16, _dbgSize++ * _fontsize, text);
    }


    void DebugGui::render(sf::RenderTarget& surface)
    {
        for (auto& i : _text)
            surface.draw(i);
        _text.clear();
        _dbgSize = 0;
    }
}
