#include "MainState.hpp"
#include "gamelib/Game.hpp"
#include "gamelib/utils/log.hpp"


MainState::MainState() :
    _game(0),
    _map(_spriteset),
    _player(*this, 64, 64)
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

    if (!_map.loadFromFile("assets/map2.json"))
        return false;

    if (!_player.loadFromFile("assets/player.json"))
        return false;

    return true;
}

void MainState::quit()
{
    _game->getEventManager().unregCallback(gamelib::SFMLEvent::id, _eventCallback, this);
    _player = Player(*this, 64, 64);
    _spriteset.destroy();
    LOG_DEBUG("MainState unloaded");
}

void MainState::update(float elapsed)
{
    _map.update(elapsed);
    _player.update(elapsed);
}

void MainState::render(sf::RenderTarget& target)
{
    _game->getWindow().clear(sf::Color::White);
    _map.render(target, math::AABBf(0, 0, _game->getWindow().getSize().x, _game->getWindow().getSize().y));
    _player.render(target);
}

const gamelib::SpriteSet& MainState::getSpriteSet() const
{
    return _spriteset;
}

const gamelib::StaticTilemap<gamelib::SpriteData>& MainState::getCollisionMap() const
{
    return _map;
}

const gamelib::Game& MainState::getGame() const
{
    return *_game;
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
        else if (sfev.key.code == sf::Keyboard::G)
        {
            me->_player.reload();
        }
        else if (sfev.key.code == sf::Keyboard::Q)
        {
            me->_game->close();
        }
    }
}
