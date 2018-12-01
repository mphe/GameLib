#include "gamelib/core/rendering/FreeCam.hpp"
#include "gamelib/core/input/InputSystem.hpp"
#include <SFML/Window/Keyboard.hpp>

namespace gamelib
{
    FreeCam::FreeCam(const std::string& name, float speed_) :
        FreeCam(name, math::Vec2f(), math::Vec2f(), speed_)
    { }

    FreeCam::FreeCam(const std::string& name, const math::Vec2f& pos, const math::Vec2f& size, float speed_) :
        Camera(name, pos, size),
        speed(speed_),
        freeze(false)
    { }

    void FreeCam::update(float elapsed)
    {
        Camera::update(elapsed);

        if (freeze)
            return;

        auto input = getSubsystem<InputSystem>();
        float speed = this->speed * elapsed;

        if (input->isKeyDown(sf::Keyboard::LShift) || input->isKeyDown(sf::Keyboard::RShift))
            speed *= 3;

        if (input->isKeyDown(sf::Keyboard::H)
                || input->isKeyDown(sf::Keyboard::A)
                || input->isKeyDown(sf::Keyboard::Left))
            move(-speed, 0);

        if (input->isKeyDown(sf::Keyboard::L)
                || input->isKeyDown(sf::Keyboard::D)
                || input->isKeyDown(sf::Keyboard::Right))
            move(speed, 0);

        if (input->isKeyDown(sf::Keyboard::J)
                || input->isKeyDown(sf::Keyboard::S)
                || input->isKeyDown(sf::Keyboard::Down))
            move(0, speed);

        if (input->isKeyDown(sf::Keyboard::K)
                || input->isKeyDown(sf::Keyboard::W)
                || input->isKeyDown(sf::Keyboard::Up))
            move(0, -speed);

        if (input->isKeyDown(sf::Keyboard::O))
            zoom += speed / 400;

        if (input->isKeyDown(sf::Keyboard::I))
            zoom -= speed / 400;
    }

}
