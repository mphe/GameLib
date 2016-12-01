#ifndef GAME_PLAYER_HPP
#define GAME_PLAYER_HPP

#include <string>
#include "gamelib/GameObject.hpp"
#include "gamelib/sprite/AnimatedSprite.hpp"
#include "gamelib/utils/JsonObject.hpp"
#include "math/geometry/Vector2.hpp"
#include "gamelib/movement/Acceleration.hpp"
#include "gamelib/movement/TileWalker.hpp"
#include "gamelib/event/SFMLEvent.hpp"

namespace gamelib
{
    template <class T>
    class StaticTilemap;
}

class MainState;

class Player : public gamelib::GameObject, public Json::JsonObject, public gamelib::TileWalker
{
    typedef gamelib::StaticTilemap<int> CollisionMap;

    public:
        Player(MainState& mainstate);
        Player(MainState& mainstate, float x, float y);

        void update(float elapsed);
        void render(sf::RenderTarget& target);

        bool loadFromJson(const Json::Value& node);

    private:
        enum State
        {
            Walk,
            Stand,
            Jump,
            StateCount
        };

    private:
        void _changeState(State state);

    private:
        struct {
            float walkspeed;
            geometry::Vector2f origin;
            const gamelib::SpriteData* spritedata[StateCount];
        } _options;

    private:
        MainState* _mainstate;
        State _state;
        gamelib::Acceleration _gravity;
        gamelib::Acceleration _jumpspeed;
        gamelib::AnimatedSprite _sprite;
        gamelib::TilemapCollider<gamelib::SpriteData> _col;
        bool _onground;
        bool _canjump;
};

/*
 * Config file structure:
 * {
 *     "walk": <walk sprite>,
 *     "stand": <stand sprite>,
 *     "jump": <jump sprite>,
 *     "walkspeed": <walk speed (pixel per second)>,
 *
 *     "minjump": <start jump speed (pps)>,
 *     "maxjump": <end jump speed (pps)>,
 *     "jumpaccl": <jump speed decrease (pps)>,
 *
 *     "mingravity": <minimal gravity>,
 *     "gravityaccl": <gravity acceleration>,
 *     "maxgravity": <maximal gravity>,
 *
 *     "width": <bbox width>,
 *     "height": <bbox height>,
 *     "originx": <origin x>,
 *     "originy": <origin y>
 * }
 */

#endif
