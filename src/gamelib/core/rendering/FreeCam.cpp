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

        if (input->isKeyDown(sf::Keyboard::LShift) || input->isKeyDown(sf::Keyboard::RShift))
            speed *= 3;

        if (input->isKeyDown(sf::Keyboard::H)
                || input->isKeyDown(sf::Keyboard::A)
                || input->isKeyDown(sf::Keyboard::Left))
            cam->move(-speed, 0);

        if (input->isKeyDown(sf::Keyboard::L)
                || input->isKeyDown(sf::Keyboard::D)
                || input->isKeyDown(sf::Keyboard::Right))
            cam->move(speed, 0);

        if (input->isKeyDown(sf::Keyboard::J)
                || input->isKeyDown(sf::Keyboard::S)
                || input->isKeyDown(sf::Keyboard::Down))
            cam->move(0, speed);

        if (input->isKeyDown(sf::Keyboard::K)
                || input->isKeyDown(sf::Keyboard::W)
                || input->isKeyDown(sf::Keyboard::Up))
            cam->move(0, -speed);

        if (input->isKeyDown(sf::Keyboard::O))
            cam->zoom(speed / 400);

        if (input->isKeyDown(sf::Keyboard::I))
            cam->zoom(-speed / 400);
    }
}
