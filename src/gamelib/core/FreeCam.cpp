#include "gamelib/core/FreeCam.hpp"
#include "gamelib/core/rendering/Scene.hpp"
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

        float speed = this->speed * elapsed;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::A)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            cam->move(-speed, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::D)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            cam->move(speed, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::S)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            cam->move(0, speed);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::W)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            cam->move(0, -speed);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
            cam->zoom += speed / 400;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
            cam->zoom -= speed / 400;
    }

}
