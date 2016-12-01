#include "Player.hpp"
#include <cassert>
#include "gamelib/utils/log.hpp"
#include "MainState.hpp"
#include "gamelib/Game.hpp"

Player::Player(MainState& mainstate) : Player(mainstate, 0, 0)
{ }

Player::Player(MainState& mainstate, float x, float y) :
    TileWalker(x, y, 0, 0, _col),
    _options{ 0 },
    _mainstate(&mainstate),
    _state(Stand),
    _onground(true),
    _canjump(true),
    _col(mainstate.getCollisionMap())
{ }


void Player::update(float elapsed)
{
    geometry::Vector2<float> motion;
    const gamelib::Game& game = _mainstate->getGame();

    // Contact ground event
    if (!_onground && !placeFreeRelative(0, 1))
    {
        _onground = true;
        _gravity.stop();
        _jumpspeed.stop();
    }

    // In air event
    if (_onground && placeFreeRelative(0, 1))
    {
        _onground = false;
        _gravity.reset();
        _changeState(Jump);
    }

    // Contact top event
    if (!placeFreeRelative(0, -1))
    {
        _gravity.reset();
        _jumpspeed.stop();
    }

    // Controls
    if (game.isKeyPressed(sf::Keyboard::A) ^ game.isKeyPressed(sf::Keyboard::D))
    {
        if (_onground)
            _changeState(Walk);

        if (game.isKeyPressed(sf::Keyboard::A))
        {
            motion.x -= _options.walkspeed * elapsed;
            _sprite.setScale(-1, 1);
        }
        else if (game.isKeyPressed(sf::Keyboard::D))
        {
            motion.x += _options.walkspeed * elapsed;
            _sprite.setScale(1, 1);
        }
    }
    else if (_onground)
    {
        _changeState(Stand);
    }

    if (_canjump && _onground && game.isKeyPressed(sf::Keyboard::W))
    {
        _canjump = false;
        _jumpspeed.reset();
    }
    else if (!game.isKeyPressed(sf::Keyboard::W))
    {
        _canjump = true;
        _jumpspeed.update(elapsed);
    }

    if (!_onground)
        _gravity.update(elapsed);

    motion.y += _gravity.speed * elapsed;
    motion.y += _jumpspeed.speed * elapsed;

    // Apply movement
    moveContact(motion.x, motion.y);

    // Update sprite
    _sprite.update(elapsed);
    _sprite.setPosition(_rect.pos.x + _options.origin.x, _rect.pos.y + _options.origin.y);
}

void Player::render(sf::RenderTarget& target)
{
    _sprite.render(target);
}

bool Player::loadFromJson(const Json::Value& node)
{
    // Must be in the same order as the State enum
    const char* strs[] = { "walk", "stand", "jump" };
    const gamelib::SpriteSet& set = _mainstate->getSpriteSet();

    for (int i = 0; i < StateCount; ++i)
    {
        if (!node.isMember(strs[i]))
        {
            LOG_ERROR("No sprite specified for '", strs[i], "'");
            return false;
        }

        auto it = set.find(node[strs[i]].asString());
        if (it == set.end())
        {
            LOG_ERROR("Sprite for '", strs[i], "' does not exist.");
            return false;
        }
        _options.spritedata[i] = &(it->second);
    }

    // Force sprite recreation in case the texture has changed
    _sprite = set.getAnimatedSprite(node[strs[Stand]].asString());

    _options.walkspeed = node.get("walkspeed", _options.walkspeed).asFloat();

    /*
     * Bounding Box:
     *
     * (0/0)    width
     *      +-----------+
     *      |           |
     *      | x         |
     *      |  origin   | height
     *      |           |
     *      |           |
     *      +-----------+
     */

    _options.origin.set(node.get("originx", 0).asFloat(), node.get("originy", 0).asFloat());
    _rect.size.set(node.get("width", 0).asFloat(), node.get("height", 0).asFloat());

    _gravity.min = node.get("mingravity", _gravity.min).asFloat();
    _gravity.max = node.get("maxgravity", _gravity.max).asFloat();
    _gravity.accl = node.get("gravityaccl", _gravity.accl).asFloat();
    _gravity.reset();

    _jumpspeed.min = node.get("minjump", _jumpspeed.min).asFloat();
    _jumpspeed.max = node.get("maxjump", _jumpspeed.max).asFloat();
    _jumpspeed.accl = node.get("jumpaccl", _jumpspeed.accl).asFloat();
    _jumpspeed.stop();

    return true;
}

void Player::_changeState(State state)
{
    if (state != _state)
    {
        _state = state;
        _sprite.change(*_options.spritedata[_state]);
    }
}
