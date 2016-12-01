#include "MainState.hpp"
#include "gamelib/Game.hpp"
#include "gamelib/utils/log.hpp"

#define CAM_MOVE_SPEED 400
#define CAM_ZOOM_SPEED 0.5


MainState::MainState() :
    _game(0),
    _map(_scene, _spriteset),
    _bgmap(_scene, _spriteset)
{ }

bool MainState::init(gamelib::Game* game)
{
    LOG_DEBUG("Init MainState...");
    _game = game;

    // Needs to be registered, even if init fails, to still be able to
    // handle reloads
    _game->getEventManager().regCallback(gamelib::SFMLEvent::id, _eventCallback, this);

    // loadFromFile logs an error message if it fails
    if (!_spriteset.loadFromFile("assets/sprites.json"))
        return false;

    if (!_scene.getCamera().loadFromFile("assets/cam.json"))
        return false;

    if (!_map.map.loadFromFile("assets/map.json"))
        return false;

    if (!_bgmap.map.loadFromFile("assets/bgmap.json"))
        return false;

    _scene.addObject(&_bgmap);
    _scene.addObject(&_bgmap.map); // Updatable
    _scene.addObject(&_map);
    _scene.addObject(&_map.map); // Updatable

    return true;
}

void MainState::quit()
{
    _game->getEventManager().unregCallback(gamelib::SFMLEvent::id, _eventCallback, this);
    _spriteset.destroy();
    _scene.destroy();
    LOG_DEBUG("MainState unloaded");
}

void MainState::update(float elapsed)
{
    float mvspeed = CAM_MOVE_SPEED * elapsed,
          zspeed = 1 - CAM_ZOOM_SPEED * elapsed;

    gamelib::Camera& cam = _scene.getCamera();

    if (_game->isKeyPressed(sf::Keyboard::H) || _game->isKeyPressed(sf::Keyboard::Left))
        cam.move(-mvspeed, 0);

    if (_game->isKeyPressed(sf::Keyboard::L) || _game->isKeyPressed(sf::Keyboard::Right))
        cam.move(mvspeed, 0);

    if (_game->isKeyPressed(sf::Keyboard::J) || _game->isKeyPressed(sf::Keyboard::Down))
        cam.move(0, mvspeed);

    if (_game->isKeyPressed(sf::Keyboard::K) || _game->isKeyPressed(sf::Keyboard::Up))
        cam.move(0, -mvspeed);

    if (_game->isKeyPressed(sf::Keyboard::I))
        cam.zoom *= zspeed;

    if (_game->isKeyPressed(sf::Keyboard::O))
        cam.zoom /= zspeed;

    _scene.update(elapsed);
}

void MainState::render(sf::RenderTarget& target)
{
    _scene.render(target);
}


void MainState::_eventCallback(MainState* me, gamelib::EventPtr ev)
{
    sf::Event& sfev = ev->get<gamelib::SFMLEvent>()->ev;
    if (sfev.type == sf::Event::KeyPressed)
    {
        if (sfev.key.code == sf::Keyboard::R)
        {
            LOG_WARN("---------------- Reloading...");
            me->_game->reload();
            me->quit();
            me->init(me->_game);
            LOG("---------------- Reloading complete");
        }
        else if (sfev.key.code == sf::Keyboard::Q)
        {
            me->_game->close();
        }
    }
}
