#include "MainState.hpp"
#include "gamelib/Game.hpp"
#include <SFML/Graphics/RenderTarget.hpp>


MainState::MainState() :
    _game(0),
    _map(_spriteset),
    _mapcol(_map),
    _luke(200, 200, 32, 32, _mapcol)
{ }

bool MainState::init(gamelib::Game* game)
{
    _game = game;

    if (!_spriteset.loadFromFile("assets/sprites.json"))
        return false;

    if (!_map.loadFromFile("assets/map.json"))
        return false;

    return true;
}

void MainState::quit()
{
    _spriteset.destroy();
}

void MainState::update(float elapsed)
{
    float walkspeed = 200 * elapsed;

    // TileWalker
    if (_game->isKeyPressed(sf::Keyboard::A))
        _luke.moveContact(-walkspeed, 0);

    if (_game->isKeyPressed(sf::Keyboard::D))
        _luke.moveContact(walkspeed, 0);

    if (_game->isKeyPressed(sf::Keyboard::S))
        _luke.moveContact(0, walkspeed);

    if (_game->isKeyPressed(sf::Keyboard::W))
        _luke.moveContact(0, -walkspeed);

    _map.update(elapsed);
}

void MainState::render(sf::RenderTarget& target)
{
    _game->getWindow().clear(sf::Color::White);

    _map.render(target, math::AABBf(0, 0, _game->getWindow().getSize().x, _game->getWindow().getSize().y));

    sf::RectangleShape walker(sf::Vector2f(32, 32));
    walker.setFillColor(sf::Color::Blue);
    walker.setOutlineColor(sf::Color::Blue);
    walker.setPosition(_luke.getPosition().x, _luke.getPosition().y);
    target.draw(walker);
}
