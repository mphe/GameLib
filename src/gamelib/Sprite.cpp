#include "gamelib/Sprite.hpp"

namespace gamelib
{
    void Sprite::update(float fps)
    {
        offset = std::fmod(offset + speed / fps, length);
        sf::IntRect rect = sfsprite.getTextureRect();
        int x = startx + (int)offset * rect.width;
        rect.left = x % sfsprite.getTexture()->getSize().x;
        rect.top = starty + x / sfsprite.getTexture()->getSize().x * rect.height,
        sfsprite.setTextureRect(rect);
    }
}
