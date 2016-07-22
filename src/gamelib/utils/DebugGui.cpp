#include "gamelib/utils/DebugGui.hpp"

namespace gamelib
{
    DebugGui::DebugGui(const sf::Font& font, unsigned int size) :
        _fontsize(size),
        _dbgSize(0),
        _color(sf::Color::Black),
        _font(&font)
    { }

    void DebugGui::setColor(const sf::Color& col)
    {
        _color = col;
    }

    void DebugGui::render(sf::RenderTarget& surface)
    {
        for (auto& i : _text)
            surface.draw(i);
        _text.clear();
        _dbgSize = 0;
    }
}
