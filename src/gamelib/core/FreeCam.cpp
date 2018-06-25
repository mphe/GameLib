#include "gamelib/core/FreeCam.hpp"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/input/InputSystem.hpp"
#include <SFML/Window/Keyboard.hpp>

namespace gamelib
{
    FreeCam::FreeCam(Scene* scene_, float speed_, unsigned int camindex_) :
        camindex(camindex_),
        speed(speed_),
        scene(scene_)
    { }

    void FreeCam::update(float elapsed)
    {
        auto cam = scene->getCamera(camindex);
        if (!cam)
            return;

        auto input = getSubsystem<InputSystem>();
        float speed = this->speed * elapsed;

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
            cam->zoom += speed / 400;

        if (input->isKeyDown(sf::Keyboard::I))
            cam->zoom -= speed / 400;
    }

}
