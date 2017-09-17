#include "gamelib/core/sprite/SpriteData.hpp"
#include <cmath>

namespace gamelib
{
    AnimationData::AnimationData() :
        length(1),
        speed(0),
        offset(0)
    { }

    void AnimationData::update(float elapsed)
    {
        if (length > 1 && speed != 0)
            offset = std::fmod(offset + speed * elapsed, length);
    }

    sf::IntRect AnimationData::getRect(int texw, int texh) const
    {
        int x = rect.left + (int)offset * rect.width;
        return sf::IntRect(x % texw, (rect.top + x / texw * rect.height) % texh, rect.width, rect.height);
    }
}
