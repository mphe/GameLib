#include "gamelib/sprite/SpriteData.hpp"

namespace gamelib
{
    AnimationData::AnimationData() :
        length(1),
        speed(0),
        offset(0)
    { }

    void AnimationData::update(float fps)
    {
        offset = std::fmod(offset + speed / fps, length);
    }

    sf::IntRect AnimationData::getRect(int texw, int texh) const
    {
        int x = rect.left + (int)offset * rect.width;
        return sf::IntRect(x % texw, (rect.top + x / texw * rect.height) % texh, rect.width, rect.height);
    }
}
