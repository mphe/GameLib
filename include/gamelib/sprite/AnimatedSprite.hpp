#ifndef GAMELIB_SPRITE_HPP
#define GAMELIB_SPRITE_HPP

#include <SFML/Graphics.hpp>
#include "../GameObject.hpp"
#include "SpriteData.hpp"

namespace gamelib
{
    class AnimatedSprite : private sf::Sprite, public GameObject
    {
        public:
            using sf::Sprite::setTexture;
            using sf::Sprite::getTexture;
            using sf::Sprite::getLocalBounds;
            using sf::Sprite::getGlobalBounds;
            using sf::Sprite::getTextureRect;
            using sf::Sprite::setScale;
            using sf::Sprite::getScale;
            using sf::Sprite::setOrigin;
            using sf::Sprite::getOrigin;
            using sf::Sprite::getTransform;
            using sf::Sprite::getInverseTransform;
            using sf::Sprite::setPosition;
            using sf::Sprite::getPosition;
            using sf::Sprite::setRotation;
            using sf::Sprite::getRotation;
            using sf::Sprite::move;
            using sf::Sprite::scale;
            using sf::Sprite::rotate;

        public:
            AnimatedSprite() {};
            AnimatedSprite(const sf::Texture& tex);
            AnimatedSprite(const AnimationData& anidata);
            AnimatedSprite(const SpriteData& spritedata);
            AnimatedSprite(const sf::Texture& tex, const AnimationData& anidata);
            AnimatedSprite(const sf::Texture& tex, const SpriteData& spritedata);
            ~AnimatedSprite() {};

            void update(float fps);
            void render(sf::RenderTarget& target) const;

            void change(const AnimationData& anidata);
            void change(const SpriteData& spritedata);

        private:
            AnimationData _anidata;

    };
}

#endif
