#include "gamelib/Scene/VimCam.hpp"
#include "gamelib/Scene/Scene.hpp"
#include <SFML/Window/Keyboard.hpp>

namespace gamelib
{
    VimCam::VimCam(Scene* scene_, float speed_, unsigned int camindex_) :
        camindex(camindex_),
        speed(speed_),
        scene(scene_)
    { }

    void VimCam::update(float elapsed)
    {
        auto& cam = scene->getCamera(camindex);
        float speed = this->speed * elapsed;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
            cam.move(-speed, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            cam.move(speed, 0);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
            cam.move(0, speed);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            cam.move(0, -speed);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
            cam.zoom += speed / 400;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
            cam.zoom -= speed / 400;
    }

}
