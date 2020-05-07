#include "gamelib/core/rendering/FreeCam.hpp"
#include "gamelib/core/rendering/Camera.hpp"
#include "gamelib/core/input/InputSystem.hpp"
#include <SFML/Window/Keyboard.hpp>

namespace gamelib
{
    FreeCamController::FreeCamController(Camera* cam_, float speed_) :
        cam(cam_),
        speed(speed_),
        freeze(false)
    { }

    void FreeCamController::update(float elapsed)
    {
        if (freeze || !cam)
            return;

        auto input = getSubsystem<InputSystem>();
        float speed = this->speed * elapsed;

        if (input->isDown(sf::Keyboard::LShift) || input->isDown(sf::Keyboard::RShift))
            speed *= 3;

        if (input->isDown(sf::Keyboard::H)
                || input->isDown(sf::Keyboard::A)
                || input->isDown(sf::Keyboard::Left))
            cam->move(-speed, 0);

        if (input->isDown(sf::Keyboard::L)
                || input->isDown(sf::Keyboard::D)
                || input->isDown(sf::Keyboard::Right))
            cam->move(speed, 0);

        if (input->isDown(sf::Keyboard::J)
                || input->isDown(sf::Keyboard::S)
                || input->isDown(sf::Keyboard::Down))
            cam->move(0, speed);

        if (input->isDown(sf::Keyboard::K)
                || input->isDown(sf::Keyboard::W)
                || input->isDown(sf::Keyboard::Up))
            cam->move(0, -speed);

        if (input->isDown(sf::Keyboard::O))
            cam->zoom(speed / 400);

        if (input->isDown(sf::Keyboard::I))
            cam->zoom(-speed / 400);
    }
}
